#include "dialog.h"

#include<QChar>
#include <QPalette>
#include <QMessageBox>

#include <QDebug>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent), timeEditValue(0), timeLeft(0), isRunning(false)
{
    timeEdit = new QTimeEdit(INITIAL_TIME, this);
    timeEdit->setDisplayFormat("hh:mm:ss");

    timeLine = new QLabel(INITIAL_TIME_LINE, this);
    timeLine->setAlignment(Qt::AlignCenter);

    startButton = new QPushButton("start", this);
    resetButton = new QPushButton("reset", this);

    timeLineUpdateTimer = new QTimer(this);

    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(timeLine, 0, 0, 1, 2);
    mainLayout->addWidget(timeEdit, 1, 0, 1, 2);
    mainLayout->addWidget(startButton, 2, 0, 1, 1);
    mainLayout->addWidget(resetButton, 2, 1, 1, 1);
    setLayout(mainLayout);

    connect(startButton, SIGNAL(pressed()), this, SLOT(startButtonPushed())); // pushed()?
    connect(resetButton, SIGNAL(pressed()), this, SLOT(resetButtonPushed()));
    connect(timeLineUpdateTimer, SIGNAL(timeout()), this, SLOT(timeLineUpdate()));
}

void Dialog::startButtonPushed()
{
    if(!isRunning){
        if (timeLeft == 0) {
            timeLeft = timeEdit->time().hour() * 3600;
            timeLeft += timeEdit->time().minute() * 60;
            timeLeft += timeEdit->time().second();
        }
        _time.start();
        timeLineUpdateTimer->start(20);
        setTimeLineTextColor(Qt::black);
        startButton->setText("suspend");
        isRunning = true;
    } else {
        timeLineUpdateTimer->stop();
        timeLeft -= _time.restart()/1000;
        startButton->setText("remuse");
        isRunning = false;
        setTimeLineTextColor(Qt::darkYellow);
      }
}

void Dialog::resetButtonPushed()
{
    timeLineUpdateTimer->stop();
    timeEdit->setTime(INITIAL_TIME);
    timeEditValue = 0;
    timeLeft = 0;
    isRunning = false;
    startButton->setText("start");
    timeLine->setText(INITIAL_TIME_LINE);
    setTimeLineTextColor(Qt::black);
}

void Dialog::timeLineUpdate()
{
    qint64 elapsed = _time.elapsed() / 1000; // in seconds
    qint64 differ = timeLeft - elapsed;
    qint64 hours = differ / 60 / 60;
    qint64 minutes = (differ / 60) - (hours * 60);
    qint64 seconds = differ - (minutes * 60) - (hours * 3600);
    QString res = QString("%1:%2:%3")
                    .arg(hours, 2, 10, QChar('0'))
                    .arg(minutes, 2, 10, QChar('0'))
                    .arg(seconds, 2, 10, QChar('0'));
    timeLine->setText(res);

    if (differ <= 0) {
        timeLineUpdateTimer->stop();
        timeLine->setText(INITIAL_TIME_LINE);
        timeLeft = 0;
        isRunning = false;
        startButton->setText("start");

        QMessageBox msg;
        msg.setText("time expired");
        msg.exec();
    } else if (differ <= 5) {
        setTimeLineTextColor(Qt::darkRed);
    }
}

template <typename Color>
    void Dialog::setTimeLineTextColor(const Color &c)
{
    QPalette palette;
    palette.setColor(QPalette::WindowText, c);
    timeLine->setPalette(palette);
}

Dialog::~Dialog()
{

}
