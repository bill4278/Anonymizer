#include <iostream>
#include "LogBrowser.h"

#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

//https://jingyan.baidu.com/article/a65957f4d2cce824e67f9b21.html
LogBrowser::LogBrowser(QWidget *parent)
	: QMainWindow(parent)
{

	ui.setupUi(this);

}

void LogBrowser::closeEvent(QCloseEvent *event)
{
	QMessageBox::StandardButton button;
	button = QMessageBox::question(this, tr("close log browser"),
		QString(tr("Do you want to close log browser ?")),
		QMessageBox::Yes | QMessageBox::No);

	if (button == QMessageBox::No) {
		event->ignore(); 
		is_open = true;
	}
	else if (button == QMessageBox::Yes) {
		event->accept();  
		is_open = false;
	}

}


void LogBrowser::printLog(QString logQStr)
{
	ui.logTextBrowser->append(logQStr);
}

void LogBrowser::printError(QString errQStr)
{
	ui.errorTextBrowser->append(errQStr);
}

