#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->topButtons->setTitleBarWidget(new QWidget());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_sim_stop_btn_clicked()
{
    this->ui->main_view->xscale *= 2;
    this->ui->main_view->update();
}


void MainWindow::on_sim_run_btn_clicked()
{
    this->ui->main_view->xscale /= 2;
    this->ui->main_view->update();
}

