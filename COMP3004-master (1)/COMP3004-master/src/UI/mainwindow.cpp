#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Controller.h"
#include <QElapsedTimer>
#include <QDebug>

MainWindow::MainWindow(Controller* c, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    controller=c;

    //create a new timer and use it to fire tick every 0.2 seconds
    tickTimer = new QTimer(this);
    connect(tickTimer, SIGNAL(timeout()), this, SLOT(tick()));
    tickTimer->start(200);
}

MainWindow::~MainWindow()
{
    delete tickTimer;
    delete ui;
}

//fires every 0.2 seconds, triggering the controller and updating the display
void MainWindow::tick(){
    controller->tick();
    updateDisplay();
}

//Update the display using values given by the controller
void MainWindow::updateDisplay(){
    //if the value of battery is changed then send the updated value to the controller
    if (controller->getBattery() != this->ui->Battery->value())
        this->ui->Battery->setValue(controller->getBattery());
    //print #s equal to the value of output bar in controller
    this->ui->Output->setText(QString("#").repeated(controller->getOutputBar()));
    //get the values of the left and right electrodes and determine if the respective boxes should be checked
    this->ui->LeftEar->setChecked(controller->getLeftEarIcon());
    this->ui->RightEar->setChecked(controller->getRightEarIcon());
    //set the value for intensity using the value stored in controller
    this->ui->Intensity->setValue(controller->getIntensity());

    //reset the group radio buttons
    this->ui->RadioDelta->setChecked(false);
    this->ui->RadioTheta->setChecked(false);
    this->ui->RadioAlpha->setChecked(false);
    this->ui->RadioBeta->setChecked(false);
    
    //check one group button based on the value of group
    switch (controller->getGroup()){
        case 0:
            this->ui->RadioDelta->setChecked(true);
            break;
        case 1:
            this->ui->RadioTheta->setChecked(true);
            break;
        case 2:
            this->ui->RadioAlpha->setChecked(true);
            break;
        case 3:
            this->ui->RadioBeta->setChecked(true);
            break;
    }
    //reset the session radio buttons
    this->ui->Radio20Min->setChecked(false);
    this->ui->Radio45Min->setChecked(false);
    this->ui->RadioCustom->setChecked(false);
    
    //activate one of the radio buttons based on the value of session
    switch (controller->getSession()){
        case 1200:
            this->ui->Radio20Min->setChecked(true);
            break;
        case 45*60:
            this->ui->Radio45Min->setChecked(true);
            break;
        case -1: //to blank display for flashing on session startup
            break;
        default:
            this->ui->RadioCustom->setChecked(true);
            break;
    }
    
    //set the display for battery based on the bool values taken from the controller
    bool* battery = controller->getBatteryDisplay();
    this->ui->BatteryRadio1->setChecked(battery[0]);
    this->ui->BatteryRadio2->setChecked(battery[1]);
    this->ui->BatteryRadio3->setChecked(battery[2]);
    this->ui->BatteryRadio4->setChecked(battery[3]);
    this->ui->BatteryRadio5->setChecked(battery[4]);
    this->ui->BatteryRadio6->setChecked(battery[5]);
    this->ui->BatteryRadio7->setChecked(battery[6]);
    this->ui->BatteryRadio8->setChecked(battery[7]);
}

bool* getBatteryDisplay(); //returns bool[8]
int getIntensity(); //for when intensity toggled before session, should change session and reset intensity to 4 so this updates GUI

//activates when the username changes
void MainWindow::on_Username_textChanged(){ //===================================================
    //update the username
    QString currUser=this->ui->Username->toPlainText();
    //get the saved sessions
    QList<SavedSession*> savedSessions = controller->getSavedSessions(currUser);
    QString grouper[4] = {"Delta","Theta","Alpha","Beta"};
    //reset the combobox
    this->ui->comboBox->clear();
    //for each session, to a max of ten, add each session to the combobox using the session's info
    for(int x=0;x<savedSessions.size();x++){
        if(x>10){
            break;
        }//generates string for output
        QString newItem = QStringLiteral("%1 %2s, %3v")
                .arg(grouper[savedSessions[x]->getGroup()])
                .arg(savedSessions[x]->getSession())
                .arg(savedSessions[x]->getIntensity());
        this->ui->comboBox->insertItem(x,newItem);
    }
}

//update the value of battery when it gets changed
void MainWindow::on_Battery_valueChanged(int strength){
    controller->setBattery((strength));
}

//when the StartSavedSession button is pressed
void MainWindow::on_StartSavedSession_clicked(){
    //get the selection from the combobox
    int currSelection = this->ui->comboBox->currentIndex();
    //reset the username textbox
    QString currUser  = this->ui->Username->toPlainText();
    //if the selection isn't empty then start a session using the saved session settings
    if (!controller->getSavedSessions(currUser).isEmpty()){
        SavedSession* savedSession = controller->getSavedSessions(currUser)[currSelection];
        controller->startSavedSession(savedSession);
    }
}

//update the session value to 1200 when the radio button is pressed
void MainWindow::on_Radio20Min_clicked(){
    controller->setSession(20*60);
}

//update the session value to 2700 when the radio button is pressed
void MainWindow::on_Radio45Min_clicked(){
    controller->setSession(45*60);
}
//take the value in the ttaxt box and set that to the session value
void MainWindow::on_RadioCustom_clicked(){
    QString text = this->ui->RadioCustomText->toPlainText();
    bool ok;
    int value = text.toInt(&ok);
    if (ok)
        controller->setSession(text.toInt());
    else
        controller->setSession(60); //default 1 minute, tells user they messed up
}

//tell the controller the start session button was pressed
void MainWindow::on_StartSession_pressed(){
    controller->startSessionPressed();
}

//when the start session button is released
void MainWindow::on_StartSession_released(){
    //get the username from the text field
    QString username = this->ui->Username->toPlainText();
    //tell the controller the session button was released and send it the username
    controller->startSessionReleased(username);
    on_Username_textChanged();
}

//these functions will update the group based on which radio button is clicked
void MainWindow::on_RadioDelta_clicked(){
    controller->setSessionGroup(0);
}

void MainWindow::on_RadioTheta_clicked(){
    controller->setSessionGroup(1);
}

void MainWindow::on_RadioAlpha_clicked(){
    controller->setSessionGroup(2);
}

void MainWindow::on_RadioBeta_clicked(){
    controller->setSessionGroup(3);
}

//send the new value of intensity to the controller
void MainWindow::on_Intensity_valueChanged(int intensity){
    controller->setIntensity(intensity);
}

//tell the controller the power button has been pressed
void MainWindow::on_Power_pressed(){
    controller->powerPressed();
}

//tell the controller the power button has been released
void MainWindow::on_Power_released(){
    controller->powerReleased();
}

//send the controller the new value of the right earpiece
void MainWindow::on_rightEarpiece_valueChanged(int strength){
    controller->setRightEarpiece(strength);
}

//send the controller the new value of the left earpiece
void MainWindow::on_leftEarpiece_valueChanged(int strength){

    controller->setLeftEarpiece(strength);
}
