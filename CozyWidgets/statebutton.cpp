#include "statebutton.h"
#include <iostream>

StateButton::StateButton(QWidget *parent) : QPushButton(parent)
{
    connect(this, &QPushButton::clicked, this, &StateButton::HandleClick);
    state = false;
}

StateButton::~StateButton() {}

void StateButton::SetUp()
{
    state = !state;
    HandleClick();
}

void StateButton::SetText0(const QString &text)
{
    text0 = text;
}

void StateButton::SetText1(const QString &text)
{
    text1 = text;
}

void StateButton::SetIcon0(const QIcon &icon)
{
    icon0 = icon;
}

void StateButton::SetIcon1(const QIcon &icon)
{
    icon1 = icon;
}

void StateButton::SetState(bool state)
{
    this->state = state;
}

void StateButton::HandleClick()
{
    QKeySequence seq = shortcut();                  //fix qt shortcut feature

    state = !state;
    setText(state ? text1 : text0);
    setIcon(state ? icon1 : icon0);
    emit StateChanged(state);

    setShortcut(seq);                               //fix qt shortcut feature
}
