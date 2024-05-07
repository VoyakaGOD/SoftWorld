#ifndef STATEBUTTON_H
#define STATEBUTTON_H

#include <QPushButton>
#include <QIcon>

class StateButton : public QPushButton
{
    Q_OBJECT


    Q_PROPERTY(QString text0 WRITE SetText0)
    Q_PROPERTY(QString text1 WRITE SetText1)
    Q_PROPERTY(QIcon icon0 WRITE SetIcon0)
    Q_PROPERTY(QIcon icon1 WRITE SetIcon1)
    Q_PROPERTY(bool state WRITE SetState)

private:
    QString text0;
    QString text1;
    QIcon icon0;
    QIcon icon1;
    bool state;

public:
    StateButton(QWidget *parent = nullptr);
    ~StateButton();
    void SetUp();
    void SetText0(const QString &text);
    void SetText1(const QString &text);
    void SetIcon0(const QIcon &icon);
    void SetIcon1(const QIcon &icon);
    void SetState(bool state);

signals:
    void StateChanged(bool state);

private slots:
    void HandleClick();
};

#endif // STATEBUTTON_H
