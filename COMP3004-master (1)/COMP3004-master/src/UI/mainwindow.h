#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QElapsedTimer>
#include <QTimer>
#include "Controller.h"
class Controller;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    private:
        //stores a pointer to the controller
        Controller* controller;
    public:
        //constructor and deconstructor
        MainWindow(Controller* c, QWidget *parent = nullptr);
        ~MainWindow();
        //updates the display
        void updateDisplay();

    private slots:
        //fires every 0.2 seconds to keep the ui updated
        void tick();

        //fires when the username is changed
        void on_Username_textChanged();

        //fires when the battery value is changes
        void on_Battery_valueChanged(int arg1);

        //fires when the start saved session button is pressed
        void on_StartSavedSession_clicked();

        //these fire when their respective group button is pressed
        void on_Radio20Min_clicked();

        void on_Radio45Min_clicked();

        void on_RadioCustom_clicked();

        //fires when start session is pressed
        void on_StartSession_pressed();

        //fires when start session is released
        void on_StartSession_released();

        //these fire when their respective radio buttons are pressed
        void on_RadioDelta_clicked();

        void on_RadioTheta_clicked();

        void on_RadioAlpha_clicked();

        void on_RadioBeta_clicked();

        //fires when intensity is changed
        void on_Intensity_valueChanged(int arg1);

        //fires when the power button is pressed
        void on_Power_pressed();

        //fires when the power button is released
        void on_Power_released();

        //these fire when the earpiece value is changed in the admin panel
        void on_rightEarpiece_valueChanged(int strength);

        void on_leftEarpiece_valueChanged(int strength);

    private:
        //keeps a pointer to the main window
        Ui::MainWindow *ui;
        //timer used by the tick function
        QTimer *tickTimer;
};
#endif // MAINWINDOW_H
