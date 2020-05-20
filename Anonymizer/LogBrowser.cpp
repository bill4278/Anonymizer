#include <iostream>
#include "LogBrowser.h"
#include "errno.h"

#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

//https://jingyan.baidu.com/article/a65957f4d2cce824e67f9b21.html
LogBrower::LogBrower(QWidget *parent)
	: QMainWindow(parent)
{

	ui.setupUi(this);

}


void LogBrower::printLog(QString logQStr)
{
	ui.textBrowser->append(logQStr);
}