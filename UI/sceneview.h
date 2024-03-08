#include <QWidget>
#include <QGraphicsView>
#include <QPainter>
#include <Physics/softscene.h>

class SceneView : public QWidget
{
    Q_OBJECT

    public:
        QPoint translation = QPoint();
        qreal xscale = 1;
        qreal yscale = 1;
        SoftScene* scene = NULL;

        PhysicalBody* selected_body;
        bool body_grabbed;
        QPoint grabded_point;

        explicit SceneView(QWidget *parent = nullptr);

        void SetViewport(QRect &rect);
        QPoint ToSceneCoordinates(QPoint point);

    protected:
        void paintEvent(QPaintEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;

};
