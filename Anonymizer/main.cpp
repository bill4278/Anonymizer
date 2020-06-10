#include "Anonymizer.h"
#include <QtWidgets/QApplication>




int main(int argc, char *argv[])
{
	qRegisterMetaType<QFileInfoList>("QFileInfoList");
	QApplication a(argc, argv);
	
	Anonymizer AnonymizerMainWindow;	
	AnonymizerMainWindow.setWindowFlags(AnonymizerMainWindow.windowFlags()&~Qt::WindowMaximizeButtonHint);    // 禁止最大化按钮
	AnonymizerMainWindow.setFixedSize(AnonymizerMainWindow.width(), AnonymizerMainWindow.height());                     // 禁止拖动窗口大小
	AnonymizerMainWindow.show();

	

	QMessageBox *msgBox;
	msgBox = new QMessageBox("Warning :",		///--这里是设置消息框标题
		"make sure you have closed files which need to be anonymized and no Chinese characters in the path !", ///--这里是设置消息框显示的内容
		QMessageBox::Warning,							///--这里是在消息框显示的图标
		QMessageBox::Yes | QMessageBox::Default,		///---这里是显示消息框上的按钮情况
		QMessageBox::No | QMessageBox::Escape,	///---这里与 键盘上的 escape 键结合。当用户按下该键，消息框将执行cancel按钮事件
		0);														///---这里是 定义第三个按钮， 该例子 只是 了显示2个按钮

	msgBox->show();

	if (msgBox->exec() == QMessageBox::No)
	{
		return 0;
		//AnonymizerMainWindow.close();

	}


	return a.exec();
}
