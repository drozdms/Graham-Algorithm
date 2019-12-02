#include "convexhullwidget.h"
#include <algorithm>
#include <set>
#pragma comment( lib, "OpenGL32.lib" )

QPoint* ConvexHullWidget::xMinPoint = new QPoint(9999999, 0);

ConvexHullWidget::ConvexHullWidget(QWidget *parent) :
    QGLWidget(parent), scale(1.0)
{
    isConvexHullComplete = false;
    setMouseTracking(true);
    pointsBuf = QList<QPoint>();
    pointSet = QStack<QPoint*>();
    conv = QStack<QPoint*>();
    QLinearGradient gradient(QPoint(50, -20), QPoint(80, 20));
    gradient.setColorAt(0.0, Qt::white);
    gradient.setColorAt(1.0, QColor(164, 238, 223));
    v = nullptr;
    w = nullptr;
    background = QBrush(QColor(255, 255, 255));
    circleBrush = QBrush(gradient);
    circlePen = QPen(Qt::black);
    circlePen.setWidth(1);
    textPen = QPen(Qt::white);
    textFont.setPixelSize(50);
    debugNext = true;
    QShortcut * shortcutReset = new QShortcut(QKeySequence(Qt::Key_Space),this,SLOT(reset()));
    QShortcut * shortcutFinish = new QShortcut(QKeySequence(Qt::Key_Right),this,SLOT(finish()));
    shortcutReset->setAutoRepeat(false);
    shortcutFinish->setAutoRepeat(false);
}


void ConvexHullWidget::initializeGL()
{
    assert(doubleBuffer());
    setAutoBufferSwap(true);
    qglClearColor(Qt::white);

    //glEnable(GL_PROGRAM_POINT_SIZE);
}


void ConvexHullWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    if (pointsBuf.size())
    {
        glPointSize(2);
        glColor3f(0.0,0.0,1.0);
        for (auto point: pointsBuf)
        {
            glBegin(GL_POINTS);
            glVertex2d(point.x(), point.y());
            glEnd();
        }
    }

    if (conv.size())
    {

        int size = conv.size();
        if (!isConvexHullComplete)
            size --;
        for (int i = 0; i < size; ++i)
        {

            glPointSize(3);
            if (isConvexHullComplete)
                glColor3f(0.0,1.0,0.0);
            else glColor3f(1.0,0.0,0.0);
            glLineWidth(1);
            glBegin(GL_LINES);
                glVertex2d(conv.at(i)->x(), conv.at(i)->y());
                glVertex2d(conv.at((i+1)%conv.size())->x(),conv.at((i+1)%conv.size())->y());
            glEnd();
            glBegin(GL_POINTS);
                glVertex2d(conv.at(i)->x(), conv.at(i)->y());
            glEnd();
        }

        if (v!= nullptr && w != nullptr)
        {
            glPushAttrib(GL_ENABLE_BIT);
            glPointSize(3);
            glLineStipple(1, 0xAAAA);
            glEnable(GL_LINE_STIPPLE);
            glBegin(GL_LINES);
            glVertex2d(conv.top()->x(), conv.top()->y());
            glVertex2d(v->x(), v->y());
            glVertex2d(v->x(), v->y());
            glVertex2d(w->x(), w->y());
            glEnd();

            glColor3f(1.0,1.0,0.0);
            glBegin(GL_POINTS);
                glVertex2d(v->x(), v->y());
                glVertex2d(w->x(), w->y());
            glEnd();

            glPopAttrib();
        }


    }

}



void ConvexHullWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button()==Qt::RightButton)
        move_point = boost::none;
    if (event->button()==Qt::LeftButton)
    {
        QPoint pos = event->pos();
        pos = screen_to_global(pos);
        pointsBuf.push_back(pos);
        updateGL();

    }

}


void ConvexHullWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
        move_point = screen_to_global(event->pos());
    updateGL();
    event->accept();
}



void ConvexHullWidget::mouseMoveEvent(QMouseEvent* event)
{
    cur_pos = screen_to_global(event->pos());

    if (move_point)
    {
            const int w = size().width();
            const int h = size().height();

            QPoint pos(event->pos().x(), event->pos().y());
            QPoint sz(w / 2, h / 2);


            QPoint diff = pos - sz;
            diff.setX(-diff.x());

            center_ = *move_point + scale * diff;
            resizeGL(w, h);
    }

    updateGL();
    event->accept();
}


void ConvexHullWidget::wheelEvent ( QWheelEvent * e )
{
   // scale+=(e->delta()/(qreal)1200);

    double old_zoom = scale;

    int delta = e->delta() / 8 / 15;
    if (delta > 0)
    {
        for (int i = 0; i != delta; ++i)
            scale /= 1.05;
    }
    else if (delta < 0)
    {
        for (int i = 0; i != delta; --i)
            scale *= 1.05;
    }

    QPoint pos(e->pos().x(), e->pos().y());
    QPoint sz(size().width() / (qreal)2, size().height() / (qreal)2);

    QPoint diff = pos - sz;
    center_ += (old_zoom - scale) * QPoint(diff.x(), diff.y());
    mouseMoveEvent(new QMouseEvent(QEvent::MouseMove, screen_to_global(e->pos()),Qt::LeftButton, nullptr, nullptr));
    resizeGL(size().width(), size().height());



   updateGL();
   e->accept();
}

void ConvexHullWidget::resizeGL(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    QPoint size = (scale / 2) * QPoint(w, h);

    QPoint left_bottom = center_ + (-size);
    QPoint right_top   = center_ + size;

    glOrtho(left_bottom.x(), right_top.x(), left_bottom.y(), right_top.y(), -1.0, 1.0);
    glViewport(0, 0, w, h);

}



QPoint ConvexHullWidget::screen_to_global(QPoint const & screen_pos) const
{                                                   // global means (0,0) is at the center
    QPoint pos(screen_pos.x(), screen_pos.y());
    QPoint sz(size().width() / (qreal)2, size().height() / (qreal)2);

    QPoint diff = pos - sz;
    diff.setY(-diff.y());
    return center_+scale * diff;
}

void ConvexHullWidget::generatePoints(int quantity)
{



        QRandomGenerator generatorStartPoint = QRandomGenerator::securelySeeded();
        for (int i=0; i<quantity; ++i)
        {
            double start_x = generatorStartPoint.bounded(size().width()-100);
            double start_y = generatorStartPoint.bounded(size().height()-100);
            QPoint poi(start_x, start_y);
            poi = screen_to_global(poi);
            pointsBuf.push_back(poi);

        }

        updateGL();

}


bool compX(QPoint a, QPoint b)
{
    if (a.x() == b.x())
        return a.y() < b.y();
    else return a.x() < b.x();
}

bool compY(QPoint a, QPoint b)
{
    return a.y() < b.y();
}

bool sortTan(QPoint* a, QPoint* b)    // sort by decreasing tan
{
    if (a == ConvexHullWidget::xMinPoint)
        return true;
    else if (b == ConvexHullWidget::xMinPoint)
        return false;
    else if (a->x() == ConvexHullWidget::xMinPoint->x())    {
       if (b->x() == ConvexHullWidget::xMinPoint->x())
             return a->y() > b->y();
       else return a->y() > ConvexHullWidget::xMinPoint -> y();
    }
    else {
    qreal tan1 = (a->y() - ConvexHullWidget::xMinPoint->y())/(qreal)(a->x() - ConvexHullWidget::xMinPoint->x());
    if (b->x() == ConvexHullWidget::xMinPoint->x())
        return b->y() < ConvexHullWidget::xMinPoint -> y();
    else {
        qreal tan2 = (b->y() - ConvexHullWidget::xMinPoint->y())/(qreal)(b->x() - ConvexHullWidget::xMinPoint->x());
        return tan1 > tan2;
       }
    }
}


void ConvexHullWidget::buildConvex()
{
    // do preprocessor
    // find point with min x
    // sort points by tan(pl, p)

    removeDuplicates();
    debugNext = true;
    isConvexHullComplete = false;
    if (pointsBuf.size() < 3)
    {
        for (auto p : pointsBuf)
            conv.push(&p);
        isConvexHullComplete = true;
        updateGL();
        return;
    }


    pointSet.clear();
    conv.clear();
    auto Xborders = std::minmax_element(pointsBuf.begin(), pointsBuf.end(), compX);
    auto Yborders = std::minmax_element(pointsBuf.begin(), pointsBuf.end(), compY);
    xMinPoint = &*Xborders.first;
    QVector<QPoint> pol;
    pol.push_back(*Xborders.first);
    pol.push_back(*Yborders.second);
    pol.push_back(*Xborders.second);
    pol.push_back(*Yborders.first);
    pol.push_back(*Xborders.first);
    PreprocessorArea centerPoly(pol);
    for (int i =0; i < pointsBuf.size(); ++i)
        if (!centerPoly.containsPoint(pointsBuf[i]))
            pointSet.push(&pointsBuf[i]);
    std::sort(pointSet.begin(), pointSet.end(), sortTan);
    v = pointSet.pop();
    w = pointSet.pop();
    conv.push_back(xMinPoint);
    QTimer *timer = new QTimer(this);
    while (true)
    {
        if (debugNext)  {
            QEventLoop loop;
            timer->start(debugSpeed);
            connect(timer, SIGNAL(timeout()), &loop, SLOT(quit()));
            loop.exec();
            updateGL();
        }

        if (is_right_turn(conv.top(), v, w))
            v = conv.pop();
        else    {
            conv.push(v);
            v = w;
            if (pointSet.size() == 0)
                break;
            w = pointSet.pop();
        }



    }

     isConvexHullComplete = true;
     v = nullptr;
     w = nullptr;
    updateGL();

}







inline qreal ConvexHullWidget::determinant(QPoint* p1, QPoint* p2, QPoint* p3) {
   return (p2->x() * p3->y() - p3->x() * p2->y()) - (p1->x() * p3->y() - p3->x() * p1->y()) +
      (p1->x() * p2->y() - p2->x() * p1->y());
}

bool ConvexHullWidget::is_right_turn(QPoint* p1, QPoint* p2,
      QPoint* p3) {
   return determinant(p1, p2, p3) <= 0;
}

void ConvexHullWidget::reset()
{
    if (!isConvexHullComplete)
        return;
    pointSet.clear();
    pointsBuf.clear();
    conv.clear();
    isConvexHullComplete = false;
}



void ConvexHullWidget::PreprocessorArea::qt_polygon_isect_line_exclusive(const QPoint &p1, const QPoint &p2, const QPoint &pos,
                                  int *winding)
{
    int x1 = p1.x();
    int y1 = p1.y();
    int x2 = p2.x();
    int y2 = p2.y();
    int y = pos.y();
    int dir = 1;
    if (qFuzzyCompare((qreal)y1, (qreal)y2)) {
        // ignore horizontal lines according to scan conversion rule
        return;
    } else if (y2 < y1) {
        int x_tmp = x2; x2 = x1; x1 = x_tmp;
        int y_tmp = y2; y2 = y1; y1 = y_tmp;
        dir = -1;
    }
    if (y >= y1 && y <= y2) {
        qreal x = x1 + ((x2 - x1) / (qreal)(y2 - y1)) * (y - y1);
        // count up the winding number if we're
        if (x < pos.x()) {
            (*winding) += dir;
        } else if (x == pos.x())    {
            *winding = INT_MAX;

        }
    }
}

bool ConvexHullWidget::PreprocessorArea::containsPoint(const QPoint &pt) const     {
    if (isEmpty())
        return false;
    int winding_number = 0;
    QPoint last_pt = at(0);
    QPoint last_start = at(0);
    for (int i = 1; i < size(); ++i) {
        const QPoint &e = at(i);
        qt_polygon_isect_line_exclusive(last_pt, e, pt, &winding_number);
        last_pt = e;
        if (winding_number == INT_MAX)
            break;
    }
    if (winding_number == INT_MAX)
        return 0;                           // we exclude vertices and edges
    // implicitly close last subpath
    if (last_pt != last_start)
        qt_polygon_isect_line_exclusive(last_pt, last_start, pt, &winding_number);
    return (winding_number % 2) != 0;
}

ConvexHullWidget::PreprocessorArea::PreprocessorArea(QVector<QPoint> pol) : QPolygon(pol)  {}


uint qHash(const QPoint &p)
{
   return qHash(QPair<int, int> (p.x(), p.y()) );
}


void ConvexHullWidget::removeDuplicates()   {

    QSet<QPoint> s;
    for (QPoint l : pointsBuf)
        s.insert(l);
    pointsBuf = QList<QPoint>();
    for (QPoint p : s)
        pointsBuf.push_back(p);
}


void ConvexHullWidget::finish() {
    debugNext = false;
}

void ConvexHullWidget::changeDebugSpeed(int val)    {
    debugSpeed = val;
}
