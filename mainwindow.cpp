#include "mainwindow.h"
#include "./ui_mainwindow.h"

//temporary:
#include "Inspector/inspectorheader.h"
#include "Inspector/inspectorintegerfield.h"
#include "Inspector/inspectorbutton.h"

static bool is_running = true;
QIcon run_icon;
QIcon stop_icon;

static void test_func()
{
    qDebug("everything was deleted!");
}

int int_param_value = 0;
QColor color_param_value = QColorConstants::Blue;
static void print_func()
{
    qDebug("int_param = %d, color_param = #%d%d%d",
           int_param_value, color_param_value.red(), color_param_value.green(), color_param_value.blue());
}
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

    //CertainInspectableParam color_param("color", InspectableParamType::Color, color_param_value);

    CertainInspectableParam int_param("integer", InspectableParamType::Int, int_param_value, 12, 25);
    new InspectorIntegerField(ui->inspectorContents, ui->inspectorLayout, int_param);
    auto d_par = new InspectorIntegerField(ui->inspectorContents, ui->inspectorLayout, int_param);

    auto d_hdr = new InspectorHeader(ui->inspectorContents, ui->inspectorLayout, "d-hdr", 3);
    new InspectorHeader(ui->inspectorContents, ui->inspectorLayout, "actions", 3);
    auto d_btn = new InspectorButton(ui->inspectorContents, ui->inspectorLayout, InspectableAction("d-btn", test_func));
    new InspectorButton(ui->inspectorContents, ui->inspectorLayout, InspectableAction("delete everything", test_func));
    new InspectorButton(ui->inspectorContents, ui->inspectorLayout, InspectableAction("print", print_func));

    delete d_btn;
    delete d_hdr;
    delete d_par;

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
