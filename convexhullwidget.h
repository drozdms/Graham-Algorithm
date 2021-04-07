#ifndef CONVEXHULLWIDGET_H
#define CONVEXHULLWIDGET_H

#include <QtOpenGL>
#include <boost/optional.hpp>
#include "Predicates.h"
class ConvexHullWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit ConvexHullWidget(QWidget *parent = nullptr);
    static QPointF* xMinPoint;
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
    REAL* v;
    REAL* w;
    QPen textPen;
    QColor current_color_;
    QList<QPointF> pointsBuf;
    QStack<REAL*> pointSet;
    QStack<REAL*> conv;
    bool isConvexHullComplete;
    QPointF center_;
    QPointF cur_pos;
    // QUERY

    boost::optional<QPointF> move_point;

    class PreprocessorArea : public QPolygonF   {

    public:

        PreprocessorArea(QVector<QPointF> pol);
        bool containsPoint(const QPointF &pt) const;

    private:
        static void qt_polygon_isect_line_exclusive(const QPointF &p1, const QPointF &p2, const QPointF &pos,
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
    inline bool is_right_turn(QPointF* p1, QPointF* p2,
          QPointF* p3);


    QPointF screen_to_global(QPointF const & screen_pos) const;
    inline qreal determinant(QPointF* p1, QPointF* p2, QPointF* p3);
};

#endif // CONVEXHULLWIDGET_H
