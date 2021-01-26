#include <iostream>
#include "Anonymizer.h"
#include "errno.h"

#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

// https://itk.org/Doxygen/html/Examples_2IO_2DicomImageReadChangeHeaderWrite_8cxx-example.html
// https://bbs.csdn.net/topics/394710257  font color
// https://zhuanlan.zhihu.com/p/91143055 卡顿
// https://blog.csdn.net/guo88455648/article/details/82736202

Anonymizer::Anonymizer(QWidget *parent)
	: QMainWindow(parent)
{

	ui.setupUi(this);
	setupConnection();

}

Anonymizer::~Anonymizer()
{
	if (m_objThreadForDcm != nullptr)
	{
		m_objThreadForDcm->quit();
		m_objThreadForDcm->wait();
		delete m_objThreadForDcm;
		m_objThreadForDcm = nullptr;
	}
	if (m_objThreadForZip != nullptr)
	{
		m_objThreadForZip->quit();
		m_objThreadForZip->wait();
		delete m_objThreadForZip;
		m_objThreadForZip = nullptr;
	}
	if (m_objThreadForNoSuf != nullptr)
	{
		m_objThreadForNoSuf->quit();
		m_objThreadForNoSuf->wait();
		delete m_objThreadForNoSuf;
		m_objThreadForNoSuf = nullptr;
	}
}

void Anonymizer::closeEvent(QCloseEvent *event)
{
	QMessageBox::StandardButton button;
	button = QMessageBox::question(this, tr("quit anonymizer"),
		QString(tr("Do you want to quit anonymizer ?")),
		QMessageBox::Yes | QMessageBox::No);

	if (button == QMessageBox::No) {
		event->ignore();
	}
	else if (button == QMessageBox::Yes) {

		event->accept();
	}
}

void Anonymizer::setupConnection()
{

	connect(ui.anoDcmBtn, SIGNAL(clicked()), this, SLOT(slot_btn_chooseFolderForDcm()));
	connect(ui.anoZipBtn, SIGNAL(clicked()), this, SLOT(slot_btn_chooseFolderForZip()));
	connect(ui.anoNofixBtn, SIGNAL(clicked()), this, SLOT(slot_btn_chooseFolderForNoSuffix()));
	connect(ui.openFolderBtn, SIGNAL(clicked()), this, SLOT(slot_btn_chooseFolder()));
	connect(ui.openFileBtn, SIGNAL(clicked()), this, SLOT(slot_btn_chooseFile()));

	connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(slot_showAbout()));
	connect(ui.actionAbout_Qt, SIGNAL(triggered()), this, SLOT(slot_showAboutQt()));

	connect(ui.showLogBtn, SIGNAL(clicked()), this, SLOT(slot_btn_collpaseLogBrowser()));
	connect(ui.rmInstitutionCheckBox, SIGNAL(stateChanged(int)), this, SLOT(slot_needRemoveInstitution()));
	

}

void Anonymizer::processingUi()
{
	ui.logTextBrowser->clear();
	ui.errorTextBrowser->clear();

	ui.statusLabel->setText("<font color='#389fff'><b>Processing ...</b></font>");
	ui.folderNameLabel->setText("");
	ui.progressBar->setValue(0);
	ui.progressBar_2->setValue(0);
	ui.progressBar_3->setValue(0);

	ui.openFolderBtn->setDisabled(true);
	ui.openFileBtn->setDisabled(true);
	ui.anoDcmBtn->setDisabled(true);
	ui.anoZipBtn->setDisabled(true);
	ui.anoNofixBtn->setDisabled(true);
	ui.actionAbout->setDisabled(true);
	ui.actionAbout_Qt->setDisabled(true);
	ui.rmInstitutionCheckBox->setDisabled(true);
}

void Anonymizer::finishUi()
{
	ui.statusLabel->setText("<font color='#389fff'><b>Finished !</b></font>");
	ui.openFolderBtn->setDisabled(false);
	ui.openFileBtn->setDisabled(false);
	ui.anoDcmBtn->setDisabled(false);
	ui.anoZipBtn->setDisabled(false);
	ui.anoNofixBtn->setDisabled(false);
	ui.actionAbout->setDisabled(false);
	ui.actionAbout_Qt->setDisabled(false);
	ui.rmInstitutionCheckBox->setDisabled(false);
}

void Anonymizer::printLog(QString logQStr)
{
	ui.logTextBrowser->append(logQStr);
}

void Anonymizer::printError(QString errQStr)
{
	ui.errorTextBrowser->append(errQStr);
	if (is_logBrowserCollpased)
	{
		setFixedSize(1124, 462);
		ui.showLogBtn->setText("\nh\ni\nd\ne\n<\nl\no\ng\n");
		is_logBrowserCollpased = false;
	}
}

void Anonymizer::fatalError(QString fatalErr)
{

	QMessageBox *msgBox;
	msgBox = new QMessageBox("Fatal Error:",
		fatalErr,
		QMessageBox::Critical,
		QMessageBox::Yes | QMessageBox::Default,
		0,
		0);
	msgBox->show();
	if (m_objThreadForDcm != nullptr)
	{
		m_objThreadForDcm->quit();
		m_objThreadForDcm->wait();
		delete m_objThreadForDcm;
	}
	if (m_objThreadForZip != nullptr)
	{
		m_objThreadForZip->quit();
		m_objThreadForZip->wait();
		delete m_objThreadForZip;
	}

	if (msgBox->exec() == QMessageBox::Yes)
	{
		QApplication* app;
		app->exit(0);
	}

}

void Anonymizer::setProgressValue(const float value, const int bar_No)
{
	switch (bar_No)
	{
	case 1:
		ui.progressBar->setValue(value);
		break;
	case 2:
		ui.progressBar_2->setValue(value);
		break;
	case 3:
		ui.progressBar_3->setValue(value);
		break;
	}
}

void Anonymizer::getProgressFinished(const bool isFinish, const int process_No)
{
	isProgressFinish = isFinish;
	if (isProgressFinish)
	{
		
		switch (process_No)
		{
		case 0:
			if (m_objThreadForDcm != nullptr)
			{
				m_objThreadForDcm->quit();
				m_objThreadForDcm->wait();
				delete m_objThreadForDcm;
				m_objThreadForDcm = nullptr;
			}
			break;
		case 1:
			if (m_objThreadForZip != nullptr)
			{
				m_objThreadForZip->quit();
				m_objThreadForZip->wait();
				delete m_objThreadForZip;
				m_objThreadForZip = nullptr;
			}
			break;
		case 2:
			if (m_objThreadForNoSuf != nullptr)
			{
				m_objThreadForNoSuf->quit();
				m_objThreadForNoSuf->wait();
				delete m_objThreadForNoSuf;
				m_objThreadForNoSuf = nullptr;
			}
			break;
		}
		if (m_objThreadForDcm == nullptr && m_objThreadForZip == nullptr && m_objThreadForNoSuf == nullptr)
		{
			finishUi();
		}
	}
	isProgressFinish = false;
}

void Anonymizer::slot_showAbout()
{
	QMessageBox::about(this, tr("About & Update"), tr("<a href = https://github.com/bill4278/Anonymizer/releases > https://github.com/bill4278/Anonymizer/releases </a>"  "<br/><br/><a href = bill4278@foxmail.com > bill4278@foxmail.com</a>"));
}

void Anonymizer::slot_showAboutQt()
{
	QMessageBox::aboutQt(this, tr("About Qt"));
}

void Anonymizer::slot_btn_collpaseLogBrowser()
{
	if (is_logBrowserCollpased)
	{
		setFixedSize(1124, 462);
		ui.showLogBtn->setText("\nh\ni\nd\ne\n<\nl\no\ng\n");
		is_logBrowserCollpased = false;
	}
	else
	{
		setFixedSize(600, 462);
		ui.showLogBtn->setText("\ns\nh\no\nw\n>\nl\no\ng\n");
		is_logBrowserCollpased = true;
	}
}

void Anonymizer::slot_btn_chooseFolderForDcm()
{
	m_objThreadForDcm = new QThread(this);
	anonyDCMThread = new threadAnonyDCM();
	anonyDCMThread->moveToThread(m_objThreadForDcm);
	connect(this, &Anonymizer::anonyDCMStart, anonyDCMThread, &threadAnonyDCM::DCMTK_anonymizeDcm);
	connect(anonyDCMThread, &threadAnonyDCM::progressStatus, this, &Anonymizer::printLog);
	connect(anonyDCMThread, &threadAnonyDCM::progressError, this, &Anonymizer::printError);
	connect(anonyDCMThread, &threadAnonyDCM::progressValue, this, &Anonymizer::setProgressValue);
	connect(anonyDCMThread, &threadAnonyDCM::fatalError, this, &Anonymizer::fatalError);
	connect(anonyDCMThread, &threadAnonyDCM::progressFinished, this, &Anonymizer::getProgressFinished);

	connect(m_objThreadForDcm, &QThread::finished, m_objThreadForDcm, &QObject::deleteLater);
	connect(m_objThreadForDcm, &QThread::finished, anonyDCMThread, &QObject::deleteLater);

	folderChoose = fileDialog->getExistingDirectory();
	if (folderChoose != NULL)
	{

		processingUi();

		ui.folderNameLabel->setText(folderChoose);

		if (m_objThreadForDcm)
		{
			m_objThreadForDcm->start();                //开启线程
			emit anonyDCMStart(folderChoose, isNeedRemoveInstitute);                  //发出信号，启动子线程的指定槽函数go
		}


	}

}

void Anonymizer::slot_btn_chooseFolderForZip()
{
	m_objThreadForZip = new QThread(this);
	anonyZipThread = new threadAnonyZip();
	anonyZipThread->moveToThread(m_objThreadForZip);
	connect(this, &Anonymizer::anonyZipStart, anonyZipThread, &threadAnonyZip::anonymizeZip);
	connect(anonyZipThread, &threadAnonyZip::progressStatus, this, &Anonymizer::printLog);
	connect(anonyZipThread, &threadAnonyZip::progressError, this, &Anonymizer::printError);
	connect(anonyZipThread, &threadAnonyZip::progressValue, this, &Anonymizer::setProgressValue);
	connect(anonyZipThread, &threadAnonyZip::fatalError, this, &Anonymizer::fatalError);
	connect(anonyZipThread, &threadAnonyZip::progressFinished, this, &Anonymizer::getProgressFinished);

	connect(m_objThreadForZip, &QThread::finished, m_objThreadForZip, &QObject::deleteLater);
	connect(m_objThreadForZip, &QThread::finished, anonyZipThread, &QObject::deleteLater);

	folderChoose = fileDialog->getExistingDirectory();
	if (folderChoose != NULL)
	{

		processingUi();

		ui.folderNameLabel->setText(folderChoose);

		if (m_objThreadForZip)
		{
			m_objThreadForZip->start();
			emit anonyZipStart(folderChoose, isNeedRemoveInstitute);
		}

		
	}

}

void Anonymizer::slot_btn_chooseFolderForNoSuffix()
{
	m_objThreadForNoSuf = new QThread(this);
	anonyNoSufThread = new threadAnonyNoSuffix();
	anonyNoSufThread->moveToThread(m_objThreadForNoSuf);
	connect(this, &Anonymizer::anonyNoSufStart, anonyNoSufThread, &threadAnonyNoSuffix::anonymizeNosuffix);
	connect(anonyNoSufThread, &threadAnonyNoSuffix::progressStatus, this, &Anonymizer::printLog);
	connect(anonyNoSufThread, &threadAnonyNoSuffix::progressError, this, &Anonymizer::printError);
	connect(anonyNoSufThread, &threadAnonyNoSuffix::progressValue, this, &Anonymizer::setProgressValue);
	connect(anonyNoSufThread, &threadAnonyNoSuffix::fatalError, this, &Anonymizer::fatalError);
	connect(anonyNoSufThread, &threadAnonyNoSuffix::progressFinished, this, &Anonymizer::getProgressFinished);

	connect(m_objThreadForNoSuf, &QThread::finished, m_objThreadForNoSuf, &QObject::deleteLater);
	connect(m_objThreadForNoSuf, &QThread::finished, anonyNoSufThread, &QObject::deleteLater);

	QString folderChoose = fileDialog->getExistingDirectory();
	if (folderChoose != NULL)
	{
		processingUi();

		ui.folderNameLabel->setText(folderChoose);

		if (m_objThreadForNoSuf)
		{
			m_objThreadForNoSuf->start();
			emit anonyNoSufStart(folderChoose, isNeedRemoveInstitute);
		}

	}
}

void Anonymizer::slot_btn_chooseFolder()
{
	m_objThreadForDcm = new QThread(this);
	m_objThreadForZip = new QThread(this);
	m_objThreadForNoSuf = new QThread(this);

	anonyDCMThread = new threadAnonyDCM();
	anonyZipThread = new threadAnonyZip();
	anonyNoSufThread = new threadAnonyNoSuffix();

	anonyDCMThread->moveToThread(m_objThreadForDcm);
	connect(this, &Anonymizer::anonyDCMStart, anonyDCMThread, &threadAnonyDCM::DCMTK_anonymizeDcm);
	connect(anonyDCMThread, &threadAnonyDCM::progressStatus, this, &Anonymizer::printLog);
	connect(anonyDCMThread, &threadAnonyDCM::progressError, this, &Anonymizer::printError);
	connect(anonyDCMThread, &threadAnonyDCM::progressValue, this, &Anonymizer::setProgressValue);
	connect(anonyDCMThread, &threadAnonyDCM::fatalError, this, &Anonymizer::fatalError);
	connect(anonyDCMThread, &threadAnonyDCM::progressFinished, this, &Anonymizer::getProgressFinished);

	connect(m_objThreadForDcm, &QThread::finished, m_objThreadForDcm, &QObject::deleteLater);
	connect(m_objThreadForDcm, &QThread::finished, anonyDCMThread, &QObject::deleteLater);

	anonyZipThread->moveToThread(m_objThreadForZip);
	connect(this, &Anonymizer::anonyZipStart, anonyZipThread, &threadAnonyZip::anonymizeZip);
	connect(anonyZipThread, &threadAnonyZip::progressStatus, this, &Anonymizer::printLog);
	connect(anonyZipThread, &threadAnonyZip::progressError, this, &Anonymizer::printError);
	connect(anonyZipThread, &threadAnonyZip::progressValue, this, &Anonymizer::setProgressValue);
	connect(anonyZipThread, &threadAnonyZip::fatalError, this, &Anonymizer::fatalError);
	connect(anonyZipThread, &threadAnonyZip::progressFinished, this, &Anonymizer::getProgressFinished);

	connect(m_objThreadForZip, &QThread::finished, m_objThreadForZip, &QObject::deleteLater);
	connect(m_objThreadForZip, &QThread::finished, anonyZipThread, &QObject::deleteLater);

	anonyNoSufThread->moveToThread(m_objThreadForNoSuf);
	connect(this, &Anonymizer::anonyNoSufStart, anonyNoSufThread, &threadAnonyNoSuffix::anonymizeNosuffix);
	connect(anonyNoSufThread, &threadAnonyNoSuffix::progressStatus, this, &Anonymizer::printLog);
	connect(anonyNoSufThread, &threadAnonyNoSuffix::progressError, this, &Anonymizer::printError);
	connect(anonyNoSufThread, &threadAnonyNoSuffix::progressValue, this, &Anonymizer::setProgressValue);
	connect(anonyNoSufThread, &threadAnonyNoSuffix::fatalError, this, &Anonymizer::fatalError);
	connect(anonyNoSufThread, &threadAnonyNoSuffix::progressFinished, this, &Anonymizer::getProgressFinished);

	connect(m_objThreadForNoSuf, &QThread::finished, m_objThreadForNoSuf, &QObject::deleteLater);
	connect(m_objThreadForNoSuf, &QThread::finished, anonyNoSufThread, &QObject::deleteLater);
	
	QString folderChoose = fileDialog->getExistingDirectory();

	if (folderChoose != NULL)
	{
		processingUi();

		ui.folderNameLabel->setText(folderChoose);

		if (m_objThreadForDcm && m_objThreadForZip && m_objThreadForNoSuf)
		{
			m_objThreadForDcm->start();
			emit anonyDCMStart(folderChoose, isNeedRemoveInstitute);

			m_objThreadForZip->start(); 
			emit anonyZipStart(folderChoose, isNeedRemoveInstitute);

			m_objThreadForNoSuf->start();
			emit anonyNoSufStart(folderChoose, isNeedRemoveInstitute);
		}

	}
}

void Anonymizer::slot_btn_chooseFile()
{
	QString filePath = fileDialog->getOpenFileName();
	if (filePath != NULL)
	{
		processingUi();
		ui.folderNameLabel->setText(filePath);
		if (filePath.contains(".zip"))
		{
			m_objThreadForZip = new QThread(this);
			anonyZipThread = new threadAnonyZip();
			anonyZipThread->moveToThread(m_objThreadForZip);
			connect(this, &Anonymizer::anonyZipStart_2, anonyZipThread, &threadAnonyZip::anonymizeZip_2);
			connect(anonyZipThread, &threadAnonyZip::progressStatus, this, &Anonymizer::printLog);
			connect(anonyZipThread, &threadAnonyZip::progressError, this, &Anonymizer::printError);
			connect(anonyZipThread, &threadAnonyZip::progressValue, this, &Anonymizer::setProgressValue);
			connect(anonyZipThread, &threadAnonyZip::fatalError, this, &Anonymizer::fatalError);
			connect(anonyZipThread, &threadAnonyZip::progressFinished, this, &Anonymizer::getProgressFinished);

			connect(m_objThreadForZip, &QThread::finished, m_objThreadForZip, &QObject::deleteLater);
			connect(m_objThreadForZip, &QThread::finished, anonyZipThread, &QObject::deleteLater);

			QFileInfoList fileList;
			fileList.append(filePath);
			QStringList filePathSegment = filePath.split("/");
			QString fileFolder = filePathSegment[0];
			for (int i = 1; i < filePathSegment.length() - 1;i++)
			{
				fileFolder = fileFolder + "/" + filePathSegment[i];
			}

			m_objThreadForZip->start();
			emit anonyZipStart_2(fileFolder, fileList, isNeedRemoveInstitute);
		}
		else
		{
			m_objThreadForDcm = new QThread(this);
			anonyDCMThread = new threadAnonyDCM();
			anonyDCMThread->moveToThread(m_objThreadForDcm);
			connect(this, &Anonymizer::anonyDCMStart_2, anonyDCMThread, &threadAnonyDCM::DCMTK_anonymizeDcm_2);
			connect(anonyDCMThread, &threadAnonyDCM::progressStatus, this, &Anonymizer::printLog);
			connect(anonyDCMThread, &threadAnonyDCM::progressError, this, &Anonymizer::printError);
			connect(anonyDCMThread, &threadAnonyDCM::progressValue, this, &Anonymizer::setProgressValue);
			connect(anonyDCMThread, &threadAnonyDCM::fatalError, this, &Anonymizer::fatalError);
			connect(anonyDCMThread, &threadAnonyDCM::progressFinished, this, &Anonymizer::getProgressFinished);

			connect(m_objThreadForDcm, &QThread::finished, m_objThreadForDcm, &QObject::deleteLater);
			connect(m_objThreadForDcm, &QThread::finished, anonyDCMThread, &QObject::deleteLater);

			QFileInfoList fileList;
			fileList.append(filePath);
			QStringList filePathSegment = filePath.split("/");
			QString fileFolder = filePathSegment[0];
			for (int i = 1; i < filePathSegment.length() - 1;i++)
			{
				fileFolder = fileFolder + "/" + filePathSegment[i];
			}


			m_objThreadForDcm->start();
			emit anonyDCMStart_2(fileFolder, fileList, isNeedRemoveInstitute);
		}
	}

}

void Anonymizer::slot_needRemoveInstitution()
{
	if (ui.rmInstitutionCheckBox->checkState() == Qt::Checked)
	{
		isNeedRemoveInstitute = true;
		std::cout << isNeedRemoveInstitute << std::endl;
	}
	else if (ui.rmInstitutionCheckBox->checkState() == Qt::Unchecked)
	{
		isNeedRemoveInstitute = false;
		std::cout << isNeedRemoveInstitute << std::endl;
	}
}