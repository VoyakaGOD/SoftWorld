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
#include "Physics/editonlybody.h"
#include "Physics/igshell.h"
#include "Physics/softscene.h"
#include "Threads/simulationthreadscontroller.h"
#include <Physics/springmassbody.h>

extern SoftScene main_scene;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->palleteContents->PostInit(ui->main_view);
    ui->topButtonsDock->setTitleBarWidget(new QWidget());
    Inspector::Mount(ui->inspectorContents, ui->inspectorLayout, ui->main_view);
    SimulationThreadsController::Mount(&main_scene, 16000, ui->main_view, 16000);
    Icons::AddIcon("settings", ":/Icons/settings.png");

    ui->palleteContents->AddPalleteItem(new SpringMassBody(), "spring");
    IGShell *example_shell = new IGShell(QVector2D(0, 0), 100, 50, DrawingStyle(Qt::darkRed, Qt::black, 3), 10000, 28000, 1, 1450);
    ui->palleteContents->AddPalleteItem(example_shell, "soft shell");
    ui->palleteContents->AddPalleteItem(new GhostBody(), "ghost");

    EditOnlyBody *ball = new EditOnlyBody(QPoint(100,300), 40, DrawingStyle(Qt::darkGreen, Qt::darkYellow, 5));
    main_scene.AddBody(ball);
    example_shell = new IGShell(QVector2D(320, 300), 100, 50, DrawingStyle(QColor(0x00be7du), QColor(0x027290u), 3), 10000, 28000, 3, 1450);
    main_scene.AddBody(example_shell);
    GhostBody *example_ghost_body = new GhostBody();
    example_ghost_body->MoveBy(QPoint(410, 50));
    main_scene.AddBody(example_ghost_body);

    ball->WidenInspectorContext();
    ui->run_stop_btn->SetUp();
    ui->inspector_btn->SetUp();
    ui->palette_btn->SetUp();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_run_stop_btn_StateChanged(bool state)
{
    if(state)
        SimulationThreadsController::Run();
    else
        SimulationThreadsController::Stop();
}

void MainWindow::on_inspector_btn_StateChanged(bool state)
{
    if(state)
        ui->inspectorDock->hide();
    else
        ui->inspectorDock->show();
}

void MainWindow::on_palette_btn_StateChanged(bool state)
{
    if(state)
        ui->palleteDock->hide();
    else
        ui->palleteDock->show();
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
