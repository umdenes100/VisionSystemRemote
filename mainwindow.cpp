#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(Controller& controller, QWidget *parent) :
    QMainWindow(parent),
    mController(controller),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_showDestinationCheckBox_clicked(bool checked) {
    mController.onShowDestinationChanged(checked);
}

void MainWindow::on_showObstaclesCheckBox_clicked(bool checked) {
    mController.onShowObstaclesChanged(checked);
}

void MainWindow::on_showCustomCoordinateCheckBox_clicked(bool checked) {
    mController.onShowCustomCoordinateChanged(checked);
}

void MainWindow::on_customXSpinBox_valueChanged(double arg1) {
    mController.onCustomXChanged(arg1);
}

void MainWindow::on_customYSpinBox_valueChanged(double arg1) {
    mController.onCustomYChanged(arg1);
}

void MainWindow::on_randomizeButton_clicked() {
    Position target = mController.onRandomize();
    ui->showDestinationCheckBox->setText(QString("Show Destination (%1,%2)")
                                         .arg(QString::number(target.x), QString::number(target.y)));
}

void MainWindow::on_applySettingsButton_clicked() {
    mController.onApplySettings(ui->cameraDeviceSpinBox->value());
}

void MainWindow::on_brightnessSlider_valueChanged(int value) {
    mController.onBrightnessChanged(value);
}

void MainWindow::on_sharpnessSlider_valueChanged(int value) {
    mController.onSharpnessChanged(value);
}

void MainWindow::on_focusSlider_valueChanged(int value) {
    mController.onFocusChanged(value);
}
