#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <UI/palleteitem.h>
#include <Physics/ghostbody.h>

//temporary:
static bool is_running = true;
QIcon run_icon;
QIcon stop_icon;
//temporary

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->palleteContents->PostInit(ui->main_view);

    ui->topButtonsDock->setTitleBarWidget(new QWidget());
    ui->topButtonsContents->layout()->setAlignment(ui->run_stop_btn, Qt::AlignHCenter);
    run_icon.addFile(QString::fromUtf8(":/Icons/run.png"), QSize(), QIcon::Normal, QIcon::Off);   //temporary
    stop_icon.addFile(QString::fromUtf8(":/Icons/stop.png"), QSize(), QIcon::Normal, QIcon::Off); //temporary
    MainWindow::on_run_stop_btn_clicked();                                                        //temporary

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_run_stop_btn_clicked()
{
    is_running = !is_running;                                               //temporary
    ui->run_stop_btn->setIcon(is_running ? stop_icon : run_icon);           //temporary
    ui->run_stop_btn->setText(is_running ? "stop" : "run");                 //temporary
}

