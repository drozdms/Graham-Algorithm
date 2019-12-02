#ifndef CONVEXHULLWIDGET_H
#define CONVEXHULLWIDGET_H

#include <QtOpenGL>
#include <boost/optional.hpp>
class ConvexHullWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit ConvexHullWidget(QWidget *parent = nullptr);
    static QPoint* xMinPoint;
signals:

public slots:
    void generatePoints(int quantity = 1000);
    void buildConvex();
    void reset();
    void finish();
    void changeDebugSpeed(int);
private:
    qreal scale;
    QBrush background;
    QBrush circleBrush;
    QFont textFont;
    QPen circlePen;
    bool debugNext;
    int debugSpeed;
    QPoint* v;
    QPoint* w;
    QPen textPen;
    QColor current_color_;
    QList<QPoint> pointsBuf;
    QStack<QPoint*> pointSet;
    QStack<QPoint*> conv;
    bool isConvexHullComplete;
    QPoint center_;
    QPoint cur_pos;
    // QUERY

    boost::optional<QPoint> move_point;

    class PreprocessorArea : public QPolygon   {

    public:

        PreprocessorArea(QVector<QPoint> pol);
        bool containsPoint(const QPoint &pt) const;

    private:
        static void qt_polygon_isect_line_exclusive(const QPoint &p1, const QPoint &p2, const QPoint &pos,
                                          int *winding);

    };




protected:
    void wheelEvent ( QWheelEvent * event ) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *) override;
    void resizeGL(int w, int h) override;
    void initializeGL() override;
    void paintGL() override;
    void removeDuplicates();
    inline bool is_right_turn(QPoint* p1, QPoint* p2,
          QPoint* p3);


    QPoint screen_to_global(QPoint const & screen_pos) const;
    inline qreal determinant(QPoint* p1, QPoint* p2, QPoint* p3);
};

#endif // CONVEXHULLWIDGET_H
