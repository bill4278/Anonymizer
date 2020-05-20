#include "Anonymizer.h"
//#include "LogBrowser.h"
#include <QtWidgets/QApplication>




int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	Anonymizer w;	
	w.show();
	QMessageBox *msgBox;
	msgBox = new QMessageBox("FYI :",		///--这里是设置消息框标题
		"make sure you have closed files which need to be anonymized and no Chinese characters in the path !", ///--这里是设置消息框显示的内容
		QMessageBox::Warning,							///--这里是在消息框显示的图标
		QMessageBox::Ok | QMessageBox::Default,		///---这里是显示消息框上的按钮情况
		QMessageBox::Cancel | QMessageBox::Escape,	///---这里与 键盘上的 escape 键结合。当用户按下该键，消息框将执行cancel按钮事件
		0);														///---这里是 定义第三个按钮， 该例子 只是 了显示2个按钮

	msgBox->show();

	if (msgBox->exec() == QMessageBox::Cancel)
	{
		w.close();
	}

	return a.exec();
}
