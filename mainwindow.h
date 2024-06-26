#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_run_stop_btn_StateChanged(bool);
    void on_clear_btn_clicked();
    void on_inspector_btn_StateChanged(bool);
    void on_palette_btn_StateChanged(bool);
    void on_save_palette_btn_clicked();
    void on_load_palette_btn_clicked();
    void on_load_scene_btn_clicked();
    void on_save_scene_btn_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
