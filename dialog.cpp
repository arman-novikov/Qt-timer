#include "dialog.h"

#include<QChar>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent), timeEditValue(0), timeLeft(0)
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
    _time.start();
    timeEditValue = timeEdit->time().msec();
    timeLineUpdateTimer->start(20);
}

void Dialog::resetButtonPushed()
{
    timeLineUpdateTimer->stop();
    timeEdit->setTime(INITIAL_TIME);
    timeEditValue = 0;
    timeLeft = 0;
    startButton->setText("start");
    timeLine->setText(INITIAL_TIME_LINE);
}

void Dialog::timeLineUpdate()
{
    qint64 elapsed = _time.elapsed();
    qint64 differ = (timeEditValue - elapsed)/1000; // in seconds
    qint64 hours = differ / 60 / 60;
    qint64 minutes = (differ / 60) - (hours * 60);
    qint64 seconds = differ - (minutes * 60) - (hours * 3600);
    QString res = QString("%1:%2:%3")
                    .arg(hours, 2, 10, QChar('0'))
                    .arg(minutes, 2, 10, QChar('0'))
                    .arg(seconds, 2, 10, QChar('0'));
    timeLine->setText(res);
}

Dialog::~Dialog()
{

}
