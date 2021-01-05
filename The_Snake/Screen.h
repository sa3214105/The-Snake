#pragma once

#include <QtWidgets/QMainWindow>
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
#include <QDockWidget>
#include <QMouseEvent>
#include <QObject>
#include <QTimer>
#include <string>

#ifndef SCR
#define SCR
#include "algorithm.h"
class Screen : public QWidget
{
	Q_OBJECT 
	public:
		Screen(QWidget *parent = Q_NULLPTR);
		~Screen();
		void uppdate();
		void keyPressEvent(QKeyEvent *event);
		void reset();
		void start();
		void pause();
		void speedUp();
		void speedLow();
		void setAuto();
		void setManual();
		QString getSpeed();
		std::vector<std::pair<std::pair<int,int>,char>> nextStep(std::pair<int,int> curr,int layer);
		std::vector<std::pair<std::pair<int,int>,char>> nextStep(std::pair<int,int> curr,int layer,std::vector<std::pair<int,int>> c_snake);
		int getSnake_length();
		std::vector<std::pair<int,int>> getSnake();
		int width=9;
		int height=9;
	protected:
		void paintEvent(QPaintEvent *);
		QTimer *timer;
	private:

		int size=20;
		int gameSpeed=20;
		std::unique_ptr<map> map_dp=std::make_unique<map>(width,height,size);
		char dire='U';
		bool automatic=false;
		std::unique_ptr<algorithm> alg;
	signals:
		void speed_change(QString speed);
};
#endif  SCR