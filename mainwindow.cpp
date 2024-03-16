#include "mainwindow.h"
#include "./ui_mainwindow.h"

//temporary:
#include "Inspector/inspectorheader.h"
#include "Inspector/inspectorbutton.h"

static bool is_running = true;
QIcon run_icon;
QIcon stop_icon;
//temporary

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->topButtonsDock->setTitleBarWidget(new QWidget());
    ui->topButtonsContents->layout()->setAlignment(ui->run_stop_btn, Qt::AlignHCenter);
    //temporary *********************************************************************************************
    run_icon.addFile(QString::fromUtf8(":/Icons/run.png"), QSize(), QIcon::Normal, QIcon::Off);
    stop_icon.addFile(QString::fromUtf8(":/Icons/stop.png"), QSize(), QIcon::Normal, QIcon::Off);
    MainWindow::on_run_stop_btn_clicked();

    new InspectorHeader(ui->inspectorContents, ui->inspectorLayout, "some object", 1);
    new InspectorHeader(ui->inspectorContents, ui->inspectorLayout, "actions", 3);
    new InspectorButton(ui->inspectorContents, ui->inspectorLayout, InspectableAction("delete everything", nullptr));

    //temporary *********************************************************************************************
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
