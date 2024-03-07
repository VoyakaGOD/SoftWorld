#include <QWidget>
#include <QGraphicsView>

class SceneView : public QGraphicsView
{
    Q_OBJECT

    public:
        explicit SceneView(QWidget *parent = nullptr);

    protected:
        void paintEvent(QPaintEvent *event) override;
};
