#pragma once
#pragma comment(lib,"rpcrt4.lib")

#pragma comment(lib,"WS2_32.lib")

//https://www.cnblogs.com/ankier/archive/2012/11/22/2782224.html

#include <QtWidgets/QMainWindow>
#include "ui_LogBrowser.h"
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QMessageBox>

class LogBrower : public QMainWindow
{
	Q_OBJECT

public:
	LogBrower(QWidget *parent = Q_NULLPTR);

	void printLog(QString logQStr);

	

private:
	Ui::LogBrower ui;



};
