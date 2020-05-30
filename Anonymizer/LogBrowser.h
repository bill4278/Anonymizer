#pragma once
#pragma comment(lib,"rpcrt4.lib")

#pragma comment(lib,"WS2_32.lib")

//https://www.cnblogs.com/ankier/archive/2012/11/22/2782224.html

#include <QtWidgets/QMainWindow>
#include "ui_LogBrowser.h"
#include <QDir>
#include <QFile>
#include <QCloseEvent>
#include <QMessageBox>

class LogBrowser : public QMainWindow
{
	Q_OBJECT

public:
	LogBrowser(QWidget *parent = Q_NULLPTR);

	void printLog(QString logQStr);
	void printError(QString errQStr);
	Ui::LogBrowser ui;
	bool is_open = true;

private:
	void closeEvent(QCloseEvent *event);//函数重写。重写基类函数

};
