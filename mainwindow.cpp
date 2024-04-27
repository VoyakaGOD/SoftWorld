#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <iostream>
#include <QMessageBox>
#include <UI/palleteitem.h>
#include <Physics/ghostbody.h>
#include <Serialize/deserialize.h>
#include <Serialize/serialize.h>
#include <Serialize/serialize_ui.h>

//temporary:
#include "Inspector/inspectorheader.h"
#include "Inspector/inspectornumericfield.h"
#include "Inspector/inspectorcolorfield.h"
#include "Inspector/inspectorbutton.h"
#include "Inspector/inspector.h"
#include "Physics/editonlybody.h"
#include "Physics/softscene.h"
#include "Threads/simulationthreadscontroller.h"

extern SoftScene main_scene;

static bool is_running = true;
QIcon run_icon;
QIcon stop_icon;

QIcon hide_icon;
QIcon show_icon;

QString dev = "everything was deleted!";

static void test_func()
{
    qDebug() << dev;
}

int int_param_value = 0;
QColor color_param_value = QColorConstants::Blue;
float float_param_value = 0;
double double_param_value = 0;
static void print_func()
{
    qDebug("int_param = %d, color_param = %s, float_param = %f, double_param = %f",
           int_param_value, color_param_value.name().toUtf8().constData(),
           float_param_value, double_param_value);

}
//temporary

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->palleteContents->PostInit(ui->main_view);

    ui->topButtonsDock->setTitleBarWidget(new QWidget());

    Inspector::Mount(ui->inspectorContents, ui->inspectorLayout, ui->main_view);
    SimulationThreadsController::Mount(&main_scene, 16000, ui->main_view, 16000);
    Icons::AddIcon("settings", ":/Icons/settings.png");

    //temporary *********************************************************************************************
    run_icon.addFile(QString::fromUtf8(":/Icons/run.png"), QSize(), QIcon::Normal, QIcon::Off);
    stop_icon.addFile(QString::fromUtf8(":/Icons/stop.png"), QSize(), QIcon::Normal, QIcon::Off);
    hide_icon.addFile(QString::fromUtf8(":/Icons/closed_lock.png"), QSize(), QIcon::Normal, QIcon::Off);
    show_icon.addFile(QString::fromUtf8(":/Icons/opened_lock.png"), QSize(), QIcon::Normal, QIcon::Off);
    MainWindow::on_run_stop_btn_clicked();

    Inspector::AddHeader("some object", LARGE_HEADER);
    Inspector::AddParam("color", color_param_value);
    Inspector::AddParam("integer", int_param_value, 12, 25);
    Inspector::AddParam("float", float_param_value, -5.0f, 5.0f);
    Inspector::AddParam("double", double_param_value, -1e3, 1e3);
    Inspector::AddParam("dev", dev);

    Inspector::AddHeader("actions", NORMAL_HEADER);
    Inspector::AddAction("delete everything", test_func);
    Inspector::AddAction("print", print_func);

    //Inspector::Clear();
    Inspector::AddHeader("----------------------", SMALL_HEADER);

    EditOnlyBody *test_body = new EditOnlyBody(QPoint(300,300), 40, DrawingStyle(Qt::darkGreen, Qt::darkYellow, 5));
    main_scene.AddBody(test_body);
    test_body->WidenInspectorContext();

    //temporary *********************************************************************************************
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_run_stop_btn_clicked()
{
    QKeySequence shortcut = ui->run_stop_btn->shortcut();                   //fix gt feature
    is_running = !is_running;                                               //temporary
    ui->run_stop_btn->setIcon(is_running ? stop_icon : run_icon);           //temporary
    ui->run_stop_btn->setText(is_running ? "stop" : "run");                 //temporary
    ui->run_stop_btn->setShortcut(shortcut);                                //fix gt feature
    if(is_running)
        SimulationThreadsController::Run();
    else
        SimulationThreadsController::Stop();
}


void MainWindow::on_inspector_btn_clicked()
{
    if(ui->inspectorDock->isHidden())
    {
        ui->inspectorDock->show();
        ui->inspector_btn->setIcon(show_icon);
    }
    else
    {
        ui->inspectorDock->hide();
        ui->inspector_btn->setIcon(hide_icon);
    }
}

void MainWindow::on_palette_btn_clicked()
{
    if(ui->palleteDock->isHidden())
    {
        ui->palleteDock->show();
        ui->palette_btn->setIcon(show_icon);
    }
    else
    {
        ui->palleteDock->hide();
        ui->palette_btn->setIcon(hide_icon);
    }
}

void MainWindow::on_save_palette_btn_clicked() {
    guiSerializeObj(this, *(this->ui->palleteContents));
}

void MainWindow::on_load_palette_btn_clicked() {
    guiDeserializeObjInplace(this, this->ui->palleteContents
        , palleteDeserialize);
}

void MainWindow::on_save_scene_btn_clicked() {
    main_scene.Lock();
    guiSerializeObj(this, main_scene);
    main_scene.Unlock();
}

void MainWindow::on_load_scene_btn_clicked() {
    Inspector::SetTarget(nullptr, nullptr);
    guiDeserializeObjInplace(this, &main_scene, deserializeInplaceFull);
    this->ui->centralwidget->update();
}

void MainWindow::on_clear_btn_clicked() {
    Inspector::SetTarget(nullptr, nullptr);
    main_scene.Clear();
    this->ui->centralwidget->update();
}
