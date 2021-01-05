#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_The_Snake.h"
#ifndef TEST
	#define TEST
	#include <QApplication>
	#include <QPainter>
	#include <QWidget>
	#include <utility>
	#include <vector>
	#include "model.h"
	#include <QTimer>
	#include <QKeyEvent>
#endif  // !test
class The_Snake : public QMainWindow
{
    Q_OBJECT

public:
    The_Snake(QWidget *parent = Q_NULLPTR);

protected:
	
private:
    Ui::The_SnakeClass ui;

};
