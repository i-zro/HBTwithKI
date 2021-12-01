#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_HBTwithKIQt.h"

class HBTwithKIQt : public QMainWindow
{
    Q_OBJECT

public:
    HBTwithKIQt(QWidget *parent = Q_NULLPTR);

private:
    Ui::HBTwithKIQtClass ui;
};
