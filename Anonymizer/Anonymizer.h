

#pragma once
#pragma comment(lib,"rpcrt4.lib")

#pragma comment(lib,"WS2_32.lib")

//https://www.cnblogs.com/ankier/archive/2012/11/22/2782224.html

#include <QtWidgets/QMainWindow>
#include "ui_Anonymizer.h"
//#include "LogBrowser.h"
#include "compressAndUncompress.h"
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QMessageBox>
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

	QFileDialog *fileDialog = new QFileDialog(this);

private:
	Ui::mainWindow ui;

// 	using ImageType = itk::Image<unsigned char, 3>;
// 	using ReaderType = itk::ImageFileReader<ImageType>;
// 	using WriterType = itk::ImageFileWriter<ImageType>;
// 	using ImageIOType = itk::GDCMImageIO;
// 	using DictionaryType = itk::MetaDataDictionary;

	void setupConnection();
	//void ITK_anonymizeDcm(QString folderChoose, QFileInfoList dcmList);
	void anonymizeZip(QString folderChoose, QFileInfoList zipList);
	void DCMTK_anonymizeDcm(QString folderChoose, QFileInfoList dcmList);
	void anonymizeNoSuffix(QString folderChoose);


	void removeFile(const char * filePath);
	void renameFile(const char * oldFilePath, const char * newFileName);
	//void anonymizeCore(DictionaryType &dictionary);
	QFileInfoList getFileList(QString folderChoose, QStringList nameFilters);



	std::string patientNameTag = "0010|0010";
	std::string patientName = "huangbiubiu123";
	std::string patientIDTag = "0010|0020";
	std::string patientID = "huangbiubiu123";
	std::string patientBirthDateTag = "0010|0030";
	std::string patientBirthDate = "huangbiubiu123";
	std::string patientBirthTimeTag = "0010|0032";
	std::string patientBirthTime = "huangbiubiu123";
	std::string patientSexTag = "0010|0040";
	std::string patientSex = "huangbiubiu123";
	std::string otherPatientIDTag = "0010|1000";
	std::string otherPatientID = "huangbiubiu123";
	std::string otherPatientNameTag = "0010|1001";
	std::string otherPatientName = "huangbiubiu123";
	std::string patientAgeTag = "0010|1010";
	std::string patientAge = "huangbiubiu123";
	std::string patientSizeTag = "0010|1020";
	std::string patientSize = "huangbiubiu123";
	std::string patientWeightTag = "0010|1030";
	std::string patientWeight = "huangbiubiu123";


	QStringList nameFiltersDcm;
	QStringList nameFiltersZip;
	QStringList nameFiltersAllSuffix;
	QStringList nameFiltersAllFile;

	//LogBrower LB;
	CZlib mZlib;

private slots:
	void slot_btn_chooseFolder();
};
