#include "convexhullwidget.h"
#include "Predicates.h"
#include <algorithm>
#include <set>
#include <iostream>
#pragma comment( lib, "OpenGL32.lib" )

QPointF* ConvexHullWidget::xMinPoint = new QPointF(9999999, 0);

const double EPS = 1e-06;

ConvexHullWidget::ConvexHullWidget(QWidget *parent) :
    QGLWidget(parent), scale(1.0)
{
    isConvexHullComplete = false;
    setMouseTracking(true);
    pointsBuf = QList<QPointF>();
    pointSet = QStack<REAL*>();
    conv = QStack<REAL*>();
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
            glLineWidth(2);
            glBegin(GL_LINES);
                glVertex2d(conv.at(i)[0], conv.at(i)[1]);
                glVertex2d(conv.at((i+1)%conv.size())[0],conv.at((i+1)%conv.size())[1]);
            glEnd();
            glBegin(GL_POINTS);
                glVertex2d(conv.at(i)[0], conv.at(i)[1]);
            glEnd();
        }

        if (v!= nullptr && w != nullptr)
        {
            glPushAttrib(GL_ENABLE_BIT);
            glPointSize(3);
            glLineStipple(1, 0xAAAA);
            glEnable(GL_LINE_STIPPLE);
            glBegin(GL_LINES);
            glVertex2d(conv.top()[0], conv.top()[1]);
            glVertex2d(v[0], v[1]);
            glVertex2d(v[0], v[1]);
            glVertex2d(w[0], w[1]);
            glEnd();

            glColor3f(1.0,1.0,0.0);
            glBegin(GL_POINTS);
                glVertex2d(v[0], v[0]);
                glVertex2d(w[0], w[1]);
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
        QPointF pos = event->pos();
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
            const double w = size().width();
            const double h = size().height();

            QPointF pos(event->pos().x(), event->pos().y());
            QPointF sz(w / 2, h / 2);


            QPointF diff = pos - sz;
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

    QPointF pos(e->pos().x(), e->pos().y());
    QPointF sz(size().width() / (qreal)2, size().height() / (qreal)2);

    QPointF diff = pos - sz;
    center_ += (old_zoom - scale) * QPointF(diff.x(), diff.y());
    mouseMoveEvent(new QMouseEvent(QEvent::MouseMove, screen_to_global(e->pos()),Qt::LeftButton, nullptr, nullptr));
    resizeGL(size().width(), size().height());



   updateGL();
   e->accept();
}

void ConvexHullWidget::resizeGL(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    QPointF size = (scale / 2) * QPointF(w, h);

    QPointF left_bottom = center_ + (-size);
    QPointF right_top   = center_ + size;

    glOrtho(left_bottom.x(), right_top.x(), left_bottom.y(), right_top.y(), -1.0, 1.0);
    glViewport(0, 0, w, h);

}



QPointF ConvexHullWidget::screen_to_global(QPointF const & screen_pos) const
{                                                   // global means (0,0) is at the center
    QPointF pos(screen_pos.x(), screen_pos.y());
    QPointF sz(size().width() / (qreal)2, size().height() / (qreal)2);

    QPointF diff = pos - sz;
    diff.setY(-diff.y());
    return center_+scale * diff;
}

void ConvexHullWidget::generatePoints(int quantity)
{
        // TODO: Generate points due different distribution laws


        QRandomGenerator generatorStartPoint = QRandomGenerator::securelySeeded();
        for (int i=0; i<quantity; ++i)
        {
            double start_x = generatorStartPoint.bounded(size().width()-100);
            double start_y = generatorStartPoint.bounded(size().height()-100);
            QPointF poi(start_x, start_y);
            poi = screen_to_global(poi);
            pointsBuf.push_back(poi);

        }

        updateGL();

}


bool compX(QPointF a, QPointF b)
{
    if (a.x() == b.x())
        return a.y() < b.y();
    else return a.x() < b.x();
}

bool compY(QPointF a, QPointF b)
{
    return a.y() < b.y();
}


bool equal(REAL* a, REAL*b) {
    return fabs(a[0] - b[0]) < EPS && fabs(a[1] - b[1]) < EPS;
}


bool sortTan(REAL* a, REAL* b)    // sort by decreasing tan (increasing from top to bottom of the stack)
{
    REAL ref[2] = {ConvexHullWidget::xMinPoint->x(), ConvexHullWidget::xMinPoint->y()};
    exact::position flag =  exact::orient2d(ref, a, b);
    if (flag == exact::on) {
        if (equal(ref, a))
            return true;
        else if (equal(ref, b))
            return false;
        else if (a[1] > ref[1])
            return (a[1] - ref[1] < b[1] - ref[1]);
        else if (a[1] < ref[1])
            return (ref[1] - a[1] > ref[1] - b[1]);
    }
    else
        return flag == exact::right;


}


void ConvexHullWidget::buildConvex()
{
    // do preprocessor
    // find point with min x
    // sort points by tan(pl, p)



    // TODO: Use Approximation Resistant Predicates

    exact::Init();
    removeDuplicates();
    debugNext = true;
    isConvexHullComplete = false;
    if (pointsBuf.size() < 3)
    {
        for (auto p : pointsBuf)    {
            REAL pt[2] = {p.x(), p.y()};
            conv.push(pt);
        }
        isConvexHullComplete = true;
        updateGL();
        return;
    }


    pointSet.clear();
    conv.clear();
    auto Xborders = std::minmax_element(pointsBuf.begin(), pointsBuf.end(), compX);
    auto Yborders = std::minmax_element(pointsBuf.begin(), pointsBuf.end(), compY);
    xMinPoint = &*Xborders.first;
    QVector<QPointF> pol;
    pol.push_back(*Xborders.first);
    pol.push_back(*Yborders.second);
    pol.push_back(*Xborders.second);
    pol.push_back(*Yborders.first);
    pol.push_back(*Xborders.first);
    PreprocessorArea centerPoly(pol);
    for (int i =0; i < pointsBuf.size(); ++i)               // preprocessing
        if (!centerPoly.containsPoint(pointsBuf[i]))    {
            REAL* pt = new REAL[2];
            pt[0] = pointsBuf[i].x();
            pt[1] = pointsBuf[i].y();
            pointSet.push(pt);
        }
    std::sort(pointSet.begin(), pointSet.end(), sortTan);
    v = pointSet.pop();
    w = pointSet.pop();
    REAL xMinPointReal[2] = {xMinPoint->x(), xMinPoint->y()};
    conv.push_back(xMinPointReal);
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

//        std::cout << v[0] << "  " << v[1] << std::endl;
        //if (is_right_turn(conv.top(), v, w))
        if (exact::orient2d(conv.top(), v, w) == exact::right)
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
//     v = nullptr;
//     w = nullptr;
    updateGL();


}







inline qreal ConvexHullWidget::determinant(QPointF* p1, QPointF* p2, QPointF* p3) {
   return (p2->x() * p3->y() - p3->x() * p2->y()) - (p1->x() * p3->y() - p3->x() * p1->y()) +
      (p1->x() * p2->y() - p2->x() * p1->y());
}

bool ConvexHullWidget::is_right_turn(QPointF* p1, QPointF* p2,
      QPointF* p3) {
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



void ConvexHullWidget::PreprocessorArea::qt_polygon_isect_line_exclusive(const QPointF &p1, const QPointF &p2, const QPointF &pos,
                                  int *winding)
{
    qreal x1 = p1.x();
    qreal y1 = p1.y();
    qreal x2 = p2.x();
    qreal y2 = p2.y();
    qreal y = pos.y();
    int dir = 1;
    if (qFuzzyCompare(y1, y2)) {
        // ignore horizontal lines according to scan conversion rule
        return;
    } else if (y2 < y1) {           // counter-clockwise: y1 comes first
        qreal x_tmp = x2; x2 = x1; x1 = x_tmp;
        qreal y_tmp = y2; y2 = y1; y1 = y_tmp;
        dir = -1;                   // downwards direction
    }
    if (y >= y1 && y <= y2) {       // consider an edge only if it cuts a line through a point and perpendicular
                                    // to the y axis
        qreal result = (y - y1)*(x2 - x1) - (pos.x() - x1)*(y2 - y1);
        // count up the winding number if we're on the right
        if (result < 0) {                  // decrease WN if the point is on the left of a downwards line
                                            // and increase WN if the point is on the right of an upwards one
            (*winding) += dir;
        } else if (qFuzzyCompare(result, 0.0))    {
            *winding = INT_MAX;
        }
    }
}

bool ConvexHullWidget::PreprocessorArea::containsPoint(const QPointF &pt) const     {
    if (isEmpty())
        return false;
    int winding_number = 0;
    QPointF last_pt = at(0);
    QPointF last_start = at(0);
    for (int i = 1; i < size(); ++i) {
        const QPointF &e = at(i);
        qt_polygon_isect_line_exclusive(last_pt, e, pt, &winding_number);
        last_pt = e;
        if (winding_number == INT_MAX)              // a point lies on some edge
            break;
    }
    if (winding_number == INT_MAX)
        return 0;                                   // lies on an edge -> outsside
    if (last_pt != last_start)                      // implicitly close last subpath
        qt_polygon_isect_line_exclusive(last_pt, last_start, pt, &winding_number);
    return winding_number != 0;
}

ConvexHullWidget::PreprocessorArea::PreprocessorArea(QVector<QPointF> pol) : QPolygonF(pol)  {}


uint qHash(const QPointF &p)
{
   return qHash(QPair<double, double> (p.x(), p.y()) );
}


void ConvexHullWidget::removeDuplicates()   {

    QSet<QPointF> s;
    for (QPointF l : pointsBuf)
        s.insert(l);
    pointsBuf.clear();
    for (QPointF p : s)
        pointsBuf.push_back(p);
}


void ConvexHullWidget::finish() {
    debugNext = false;
}

void ConvexHullWidget::changeDebugSpeed(int val)    {
    debugSpeed = val;
}
