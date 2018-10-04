#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTimer>
#include <QLabel>
#include <QLineEdit>
#include <QTimeEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QTime>

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();
private:
    QLabel  *timeLine;
    QTimeEdit *timeEdit;
    QTime _time;
    QPushButton *startButton, *resetButton;
    QGridLayout *mainLayout;
    QTimer *timeLineUpdateTimer;
    qint64 timeEditValue;
    qint64 timeLeft;
private slots:
    void timeLineUpdate();
    void startButtonPushed();
    void resetButtonPushed();
};

static const QTime INITIAL_TIME(0, 0, 0);
static const QString INITIAL_TIME_LINE("00:00:00");

#endif // DIALOG_H
