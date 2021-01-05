#include "The_Snake.h"
#include <QtWidgets/QApplication>
#include "model.h"
#include <QLabel>
#include <QTimer>
#include <QKeyEvent>
#include <QImage>
#include "Screen.h"
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	//物件宣告定義
    The_Snake w;
	auto *game_screen=new Screen();
	QMenuBar *menuBar = new QMenuBar(nullptr);
	auto *speedLable=new QLabel(game_screen->getSpeed());
	auto *Widget=new QDockWidget();

	game_screen->setFocusPolicy (Qt::StrongFocus);
	game_screen->resize(200, 200);

	menuBar->setStyleSheet("background: #b3c6ff;");
	auto start=menuBar->addAction("start");
	auto pause=menuBar->addAction("pause");
	auto reset=menuBar->addAction("reset");
	auto speed=menuBar->addMenu("speed");
	auto increaseSpeed=speed->addAction("+");
	auto decreaseSpeed=speed->addAction("-");
	auto mode=menuBar->addMenu("mode");
	auto manual=mode->addAction("manual");
	auto automatic=mode->addAction("auto");
	QAction::connect(reset,&QAction::triggered,game_screen,&Screen::reset);
	QAction::connect(start,&QAction::triggered,game_screen,&Screen::start);
	QAction::connect(pause,&QAction::triggered,game_screen,&Screen::pause);
	QAction::connect(increaseSpeed,&QAction::triggered,game_screen,&Screen::speedUp);
	QAction::connect(decreaseSpeed,&QAction::triggered,game_screen,&Screen::speedLow);
	QAction::connect(manual,&QAction::triggered,game_screen,&Screen::setManual);
	QAction::connect(automatic,&QAction::triggered,game_screen,&Screen::setAuto);

	w.setMenuWidget(menuBar);
	Widget->setTitleBarWidget(speedLable);
	Widget->setFloating(false);
	Widget->setFeatures(0);
	Screen::connect(game_screen,&Screen::speed_change,speedLable,&QLabel::setText);
	w.setCentralWidget(game_screen);
	w.addDockWidget(Qt::RightDockWidgetArea,Widget);
	w.resize(350, 350);
    w.show();
	
    return a.exec();
}

