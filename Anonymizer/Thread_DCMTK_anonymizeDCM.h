#pragma once

#include <QObject>
//https://blog.csdn.net/u012372584/article/details/80509217
class threadAnonyDCM :public QObject
{
	Q_OBJECT
public:
	threadAnonyDCM(QObject *parent = nullptr);
	void run();
signals:
	void over();
public slots:
};