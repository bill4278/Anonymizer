#pragma once
#pragma comment(lib,"rpcrt4.lib")

#pragma comment(lib,"WS2_32.lib")

//https://www.cnblogs.com/ankier/archive/2012/11/22/2782224.html

#include <QtWidgets/QMainWindow>
#include "ui_Anonymizer.h"
#include "compressAndUncompress.h"
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QCloseEvent>
#include <QThread>
#include <QMetaType>

#include "Thread_DCMTK_anonymizeDCM.h"
#include "Thread_anonymizeZip.h"
#include "Thread_anonymizeNoSuffix.h"

//#include <QFuture>
//#include <QtConcurrent>

// #include "itkImage.h"
// #include "itkImageFileReader.h"
// #include "itkImageFileWriter.h"
// #include "itkObjectFactoryBase.h"
// #include "itkGDCMImageIO.h"
// #include "itkMetaImageIOFactory.h"
// #include "itkMetaDataObject.h"

#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmjpeg/djdecode.h"
#include "dcmtk/dcmdata/dcrledrg.h"



class Anonymizer : public QMainWindow
{
	Q_OBJECT

public:
	Anonymizer(QWidget *parent = Q_NULLPTR);
	Anonymizer::~Anonymizer();
	QFileDialog *fileDialog = new QFileDialog(this);



private:
	Ui::mainWindow ui;

	void setupConnection();

	void processingUi();
	void finishUi();

	threadAnonyDCM* anonyDCMThread = nullptr;
	threadAnonyZip* anonyZipThread = nullptr;
	threadAnonyNoSuffix* anonyNoSufThread = nullptr;


	QThread* m_objThreadForDcm = nullptr;
	QThread* m_objThreadForZip = nullptr;
	QThread* m_objThreadForNoSuf = nullptr;
	QThread* m_objThreadForFolder = nullptr;


	QString folderChoose;

	commonOperation m_coP;

	bool is_logBrowserCollpased = true;
	bool isProgressFinish = false;

signals:
	void anonyDCMStart(const QString);
	void anonyZipStart(const QString);

	void anonyDCMStart_2(const QString, const QFileInfoList fileList);
	void anonyZipStart_2(const QString, const QFileInfoList fileList);

	void anonyNoSufStart(const QString);

private slots:
	void slot_btn_chooseFolderForDcm();
	void slot_btn_chooseFolderForZip();
	void slot_btn_chooseFolderForNoSuffix();
	void slot_btn_chooseFolder();
	void slot_btn_chooseFile();
	void printLog(QString logQStr);
	void printError(QString logQStr);
	void slot_btn_collpaseLogBrowser();
	void setProgressValue(const float value, const int bar_No);
	void getProgressFinished(const bool isFinish, const int process_No);
	void fatalError(QString);
	void closeEvent(QCloseEvent *event);
	void slot_showAbout();
	void slot_showAboutQt();

};
Q_DECLARE_METATYPE(QFileInfoList);
