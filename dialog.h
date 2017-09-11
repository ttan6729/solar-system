#ifndef DIALOG_H
#define DIALOG_H

#include "config.h"
#include "universecomponent.h"
#include "zodiac.h"
#include <QDialog>
#include <QTimer>
#include <list>
#include <memory>
#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QSharedPointer>
#include <iostream>
namespace Ui {
class Dialog;

}
class EventHandler;

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    virtual ~Dialog();
    void speed_up() { m_rate += 0.2; }   //increase timestep
    void slow_down() { if(m_rate >0.2) m_rate-=0.2; } //decrease timestep

private slots:
    //prepare the next frame, called by the timer
    void nextFrame();
    //toggle the simultation's pause state
    void togglePause();
    //toggle rendering of Zodiacs
    void toggleZodiacs();
    //toggle rendering of labels
    void toggleLabels();

private:
    //method called when the window is being redrawn
    void paint(QPaintEvent *event);
    //pause (or unpause) the simulation
    void pause(bool pause);
    //handle key presses
//    void keyPressEvent(QKeyEvent *event);
    bool event(QEvent *event);

private:
    Ui::Dialog* ui;
    QTimer* m_timer; //Timer object for triggering updates

    //buttons for UI
    QPushButton* m_buttonPause;
    QPushButton* m_buttonZodiacs;
    QPushButton* m_buttonLabels;

    double m_rate = 1; //rate of the timestep
    int m_width; //width of the window
    int m_height; //height of the window
    bool m_paused; //is the simulation paused?
    bool m_renderZodiacs; //should Zodiacs be rendered?
    bool m_renderLabels; //should labels be rendered?
    long m_timestamp; //simulation time since simulation start
    UniverseComponent* m_universe; //The universe
    std::list<Zodiac>* m_zodiacs; //Vector of zodiac lines
    Config* m_config; //the singleton config instance
    EventHandler *m_handler;
};


class EventHandler
{
public:
    EventHandler(): m_next(0) {}
    virtual ~EventHandler() {}

    virtual void handleEvent(QEvent *event,Dialog *dialog) = 0;
    virtual void setNextEventHandler(EventHandler * handler)
    {
        m_next = QSharedPointer<EventHandler>(handler);
    }

protected:
    QSharedPointer<EventHandler> m_next;
};

class KeyPressEventHandler : public EventHandler
{
public:
    KeyPressEventHandler() {}
    virtual ~KeyPressEventHandler() {}

    void handleEvent(QEvent *event,Dialog *dialog);

private:
    void handleKeyPress(QKeyEvent *keyEvent,Dialog *dialog);

    bool m_moving;
    int m_direction;
};

//class MousePressEventHandler: public EventHandler
//{
//public:
//    MousePressEventHandler() {}
//    virtual ~MousePressEventHandler(){}

//    void handleEvent(QEvent *event,Dialog *dialog);

//    void handleMousePressEvent

//};

#endif // DIALOG_H
