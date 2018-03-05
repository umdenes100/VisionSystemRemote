#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "Controller.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Controller& controller, QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_showDestinationCheckBox_clicked(bool checked);
    void on_showObstaclesCheckBox_clicked(bool checked);
    void on_showCustomCoordinateCheckBox_clicked(bool checked);
    void on_customXSpinBox_valueChanged(double arg1);
    void on_customYSpinBox_valueChanged(double arg1);
    void on_randomizeButton_clicked();

    void on_applySettingsButton_clicked();
    void on_brightnessSlider_valueChanged(int value);
    void on_sharpnessSlider_valueChanged(int value);
    void on_focusSlider_valueChanged(int value);

private:
    Controller& mController;
    Ui::MainWindow *ui;


};

#endif // MAINWINDOW_H
