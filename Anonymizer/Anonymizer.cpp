#include <iostream>
#include "Anonymizer.h"

//#include "testZip.h"
#include "errno.h"

#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

// https://itk.org/Doxygen/html/Examples_2IO_2DicomImageReadChangeHeaderWrite_8cxx-example.html
// https://bbs.csdn.net/topics/394710257  font color

Anonymizer::Anonymizer(QWidget *parent)
	: QMainWindow(parent)
{

	ui.setupUi(this);
	setupConnection();

}

void Anonymizer::setupConnection()
{

	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(slot_btn_chooseFolder()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(slot_btn_chooseFolderForDcm()));
	connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(slot_btn_chooseFolderForZip()));
	connect(ui.pushButton_4, SIGNAL(clicked()), this, SLOT(slot_btn_chooseFolderForNoSuffix()));

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
		if (LogBrowserWindow.is_open)
		{
			LogBrowserWindow.close();
		}		
		event->accept();
	}
}


QFileInfoList Anonymizer::getFileList(QString folderChoose , QStringList nameFilters)
{
	QString message;

	QDir dir(folderChoose);
	QFileInfoList file_list = dir.entryInfoList( nameFilters, QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

	for (int i = 0; i != folder_list.size(); i++)
	{
		QString filePath = folder_list.at(i).absoluteFilePath();
		QFileInfoList child_file_list = getFileList(filePath, nameFilters);
		file_list.append(child_file_list);
	}

	return file_list;

}

void Anonymizer::removeFile(const char * filePath)
{
	if (std::remove(filePath) == -1)
	{
		std::cout << "Error: " << strerror(errno) << std::endl;
		LogBrowserWindow.printLog(mZlib.str2qstr("<font color = 'red'><b> error to remove: </b></font>") + mZlib.str2qstr(std::string(filePath)));
		LogBrowserWindow.printLog(mZlib.str2qstr(strerror(errno)));

		LogBrowserWindow.printError(mZlib.str2qstr("<font color = 'red'><b> error to remove: </b></font>") + mZlib.str2qstr(filePath));
		LogBrowserWindow.printError(mZlib.str2qstr(strerror(errno)));
	}
}

void Anonymizer::renameFile(const char * oldFilePath, const char * newFileName)
{
	if (std::rename(oldFilePath, newFileName) == -1)
	{
		std::cout << "Error: " << strerror(errno) << std::endl;
		LogBrowserWindow.printLog(mZlib.str2qstr("<font color = 'red'><b> error to rename: </b></font>") + mZlib.str2qstr(std::string(oldFilePath)));
		LogBrowserWindow.printLog(mZlib.str2qstr(strerror(errno)));

		LogBrowserWindow.printError(mZlib.str2qstr("<font color = 'red'><b> error to rename: </b></font>") + mZlib.str2qstr(oldFilePath));
		LogBrowserWindow.printError(mZlib.str2qstr(strerror(errno)));
	}
}

// void Anonymizer::anonymizeCore(DictionaryType &dictionary)
// {
// 	itk::EncapsulateMetaData<std::string>(dictionary, patientNameTag, patientName);
// 	itk::EncapsulateMetaData<std::string>(dictionary, patientIDTag, patientID);
// 	itk::EncapsulateMetaData<std::string>(dictionary, patientBirthDateTag, patientBirthDate);
// 	itk::EncapsulateMetaData<std::string>(dictionary, patientBirthTimeTag, patientBirthTime);
// 	itk::EncapsulateMetaData<std::string>(dictionary, patientSexTag, patientSex);
// 	itk::EncapsulateMetaData<std::string>(dictionary, otherPatientIDTag, otherPatientID);
// 	itk::EncapsulateMetaData<std::string>(dictionary, otherPatientNameTag, otherPatientName);
// 	itk::EncapsulateMetaData<std::string>(dictionary, patientAgeTag, patientAge);
// 	itk::EncapsulateMetaData<std::string>(dictionary, patientSizeTag, patientSize);
// 	itk::EncapsulateMetaData<std::string>(dictionary, patientWeightTag, patientWeight);
// }

// void Anonymizer::ITK_anonymizeDcm(QString folderChoose,QFileInfoList dcmList)
// {
// 	QDir dir(folderChoose);
// 	if (dcmList.size() == 0)
// 	{
// 		ui.progressBar->setValue(100);
// 	}
// 	for (int i = 0; i != dcmList.size(); i++)
// 	{
// 		ui.progressBar->setValue(100 * (i + 1) / dcmList.size());
// 
// 
// 		ReaderType::Pointer reader = ReaderType::New();
// 		WriterType::Pointer writer = WriterType::New();
// 
// 		std::string dcmPath_str = dcmList.at(i).absoluteFilePath().toStdString();
// 		const char* dcmPath_char = dcmPath_str.c_str();
// 
// 		itk::ObjectFactoryBase::RegisterFactory(itk::MetaImageIOFactory::New());
// 		
// 		ImageIOType::Pointer ImageIO = ImageIOType::New();
// 
// 		reader->SetImageIO(ImageIO);
// 		reader->SetFileName(dcmPath_char);    
// 		try {
// 			reader->Update();
// 		}
// 		catch(itk::ExceptionObject &err)
// 		{
// 			std::cerr << "ExceptionObject Caught" << std::endl;
// 			std::cerr << err << std::endl;
// 			std::cout << "can not read file: " << dcmPath_char << std::endl;
// 			//LogBrowserWindow.printLog(mZlib.str2qstr("can not read file: "+ std::string(dcmPath_char)));
// 		}
// 		
// 		ImageType::Pointer inputImage = reader->GetOutput();
// 		DictionaryType & dictionary = inputImage->GetMetaDataDictionary();
// 
// 		anonymizeCore(dictionary);
// 		
// 		writer->SetImageIO(ImageIO);
// 		writer->SetFileName(dcmPath_char);    
// 		writer->SetInput(reader->GetOutput());
// 		try {
// 			writer->Update();
// 		}
// 		catch (itk::ExceptionObject &err) {
// 			std::cerr << "ExceptionObject Caught" << std::endl;
// 			std::cerr << err << std::endl;
// 			std::cout << dcmPath_char << "anonymize failed !!!!!" << std::endl;
// 			//LogBrowserWindow.printLog(mZlib.str2qstr(std::string(dcmPath_char) + "anonymize failed !!!!!"));
// 		}
// 
// 		std::cout << dcmPath_char<< "  anonymized" << std::endl;
// 		//LogBrowserWindow.printLog(mZlib.str2qstr(std::string(dcmPath_char) + "  anonymized"));
// 	}
// 	/*QStringList files = dir.entryList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);
// 	DcmFileFormat fileformat;
// 	OFCondition status = fileformat.loadFile("");
// 	DcmDataset *dataset = fileformat.getDataset();
// 	OFString PatientName;
// 	dataset->putAndInsertString(DCM_PatientName, "NoOne");
// 	status = fileformat.saveFile("");
// 	dataset->findAndGetOFString(DCM_PatientName, PatientName);
// 	std::cout << PatientName;*/
// }

void Anonymizer::DCMTK_anonymizeDcm(QString folderChoose, QFileInfoList dcmList)
{
	QString createDir = folderChoose + "/dcm_temp/";
	QDir dir;
	CZlib zlib;
	if (!dir.exists(createDir))
	{
		bool res = dir.mkdir(createDir);
		if (res)
		{
			std::cout << "create folder: " << createDir.toStdString() << std::endl;
			LogBrowserWindow.printLog(mZlib.str2qstr("<font color = '#389fff'>create folder</font>: " + createDir.toStdString()));
		}
	}
	else
	{
		dir.rmdir(createDir);
	}
	//QDir dir(folderChoose);
	if (dcmList.size() == 0)
	{
		ui.progressBar->setValue(100);
	}
	for (int i = 0; i != dcmList.size(); i++)
	{
		QApplication::processEvents();
		ui.progressBar->setValue(100 * (i + 1) / dcmList.size());

		std::string dcmPath_str = dcmList.at(i).absoluteFilePath().toStdString();
		const char* dcmPath_char = dcmPath_str.c_str();

		OFCondition status;
		DcmFileFormat fileformat;
		//https://www.cnblogs.com/bayzhang/p/5484321.html
		DcmMetaInfo * metainfo;
		status = fileformat.loadFile(dcmPath_char);
		if (status.good())
		{
			std::cout << "loading: "<< dcmPath_char << std::endl;
			LogBrowserWindow.printLog(mZlib.str2qstr("<font color = '#389fff'>loading: </font>" + std::string(dcmPath_char)));
		
			DcmDataset * dataset = fileformat.getDataset();
			dataset->putAndInsertString(DCM_PatientName, "Anonymous");
			dataset->putAndInsertString(DCM_PatientID, "	");
			dataset->putAndInsertString(DCM_PatientSex, "	");
			dataset->putAndInsertString(DCM_PatientAge, "	");
			dataset->putAndInsertString(DCM_PatientBirthDate, "	");
		}
		else
		{
			LogBrowserWindow.printLog(mZlib.str2qstr("<font color = 'red'><b> error to load: </b></font>") + mZlib.str2qstr(std::string(dcmPath_char)));
			LogBrowserWindow.printError(mZlib.str2qstr("<font color = 'red'><b> error to load: </b></font>") + mZlib.str2qstr(std::string(dcmPath_char)));
		}
		status = fileformat.saveFile(zlib.qstr2str(createDir+"temp.dcm").c_str());
		removeFile(dcmPath_char);
		renameFile(zlib.qstr2str(createDir + "temp.dcm").c_str(), dcmPath_char);
		
		if (status.good())
		{
			std::cout << "anonymized: " << dcmPath_char << std::endl;
			LogBrowserWindow.printLog(mZlib.str2qstr("<font color = '#389fff'><b>anonymized: </b></font>" + std::string(dcmPath_char)));
		}
		else
		{
			LogBrowserWindow.printLog(mZlib.str2qstr("<font color = 'red'><b> error to anonymize: </b></font>") + mZlib.str2qstr(std::string(dcmPath_char)));
			LogBrowserWindow.printError(mZlib.str2qstr("<font color = 'red'><b> error to anonymize: </b></font>") + mZlib.str2qstr(std::string(dcmPath_char)));
		}

		
	}
	if (dir.rmdir(createDir))
	{
		LogBrowserWindow.printLog(mZlib.str2qstr("<font color = '#389fff'>remove folder: </font>" + mZlib.qstr2str(createDir)));
	}
	else
	{
		LogBrowserWindow.printLog(mZlib.str2qstr("<font color = 'red'><b> error to remove folder: </b></font>" + mZlib.qstr2str(createDir)));
		LogBrowserWindow.printError(mZlib.str2qstr("<font color = 'red'><b> error to remove folder: </b></font>" + mZlib.qstr2str(createDir)));
	}
	
}

void Anonymizer::anonymizeZip(QString folderChoose, QFileInfoList zipList)
{
	QString createUnzipDir = folderChoose + "/unzip_temp/";
	QDir dir;

		
	std::string createUnzipDir_str = mZlib.qstr2str(createUnzipDir);
	const char *uncompressPath_char = createUnzipDir_str.c_str();
	std::cout << uncompressPath_char << std::endl;

	QString createZipDir = folderChoose + "/zip_temp.zip";


	std::string createZipDir_str = mZlib.qstr2str(createZipDir);
	const char *compressPath_char = createZipDir_str.c_str();
	std::cout << compressPath_char << std::endl;



	if (zipList.size() == 0)
	{
		ui.progressBar_2->setValue(100);
	}
	for (int i = 0; i != zipList.size(); i++)
	{
		if (!dir.exists(createUnzipDir))
		{
			bool res = dir.mkdir(createUnzipDir);
			if (res)
			{
				std::cout << "create folder: " << createUnzipDir.toStdString() << std::endl;
				LogBrowserWindow.printLog(mZlib.str2qstr("<font color = '#389fff'>create folder: </font>" + createUnzipDir.toStdString()));
			}
		}
		else
		{
			dir.rmdir(createUnzipDir);
		}

		QApplication::processEvents();
		ui.progressBar_2->setValue(100 * (i + 1) / zipList.size());
		std::string zipPath_str = zipList.at(i).absoluteFilePath().toStdString();
		std::cout << "loading zip file " << zipPath_str << std::endl;
		int succd = mZlib.UnCompress(zipPath_str.c_str(), uncompressPath_char);
		if (succd == 1)
		{
			std::cout << "uncompressed : " << zipPath_str << std::endl;
			LogBrowserWindow.printLog(mZlib.str2qstr("<font color = '#389fff'>uncompressed: </font>" + std::string(zipPath_str)));
		}
		else
		{
			std::cout << "!!!!!  failed uncompressed : " << zipPath_str << std::endl;
			LogBrowserWindow.printLog(mZlib.str2qstr("<font color = 'red'><b> error to uncompress : </b></font>" + std::string(zipPath_str)));
			LogBrowserWindow.printError(mZlib.str2qstr("<font color = 'red'><b> error to uncompress : </b></font>" + std::string(zipPath_str)));
		}

		QFileInfoList zipDcmList;

		if (succd)
		{
			nameFiltersDcm << "*.dcm";
			zipDcmList = getFileList(uncompressPath_char, nameFiltersDcm);
			if (zipDcmList.size() == 0)
			{
				ui.progressBar->setValue(100);
			}
			else
			{
				DCMTK_anonymizeDcm(uncompressPath_char, zipDcmList);





				nameFiltersAllSuffix << "*.*";
				QFileInfoList zipFileList = getFileList(uncompressPath_char, nameFiltersAllSuffix);

				mZlib.Compress(zipFileList, compressPath_char, uncompressPath_char);
			}
		}
		
// 		d.setFilter(QDir::Files);
// 		int j, k = d.count() - 1;
// 		for (j = 0;j <= k;j++)
// 		{
// 			if (! d.remove(d[j]))
// 			{
// 				LogBrowserWindow.printLog(mZlib.str2qstr("<font color = 'red'><b> error to remove file: </b><font>") + d[j]);
// 				LogBrowserWindow.printError(mZlib.str2qstr("<font color = 'red'><b> error to remove file: </b><font>") + d[j]);
// 			}
// 		}
		QDir d(uncompressPath_char);
		if (d.removeRecursively())
		{
			std::cout << "removed temp files" << std::endl;
			LogBrowserWindow.printLog(mZlib.str2qstr("<font color = '#389fff'>removed temp files</font>"));
		}

		if (zipDcmList.size() != 0)
		{
			removeFile(zipPath_str.c_str());
			renameFile(compressPath_char, zipPath_str.c_str());
		}
	}
	//dir.rmdir(uncompressPath_char);// this method only suitable for empty folder
}

void Anonymizer::anonymizeNoSuffix(QString folderChoose)
{
	nameFiltersAllFile << "*";
	QFileInfoList AllFilesList = getFileList(folderChoose, nameFiltersAllFile);

	nameFiltersAllSuffix << "*.*";
	QFileInfoList AllSuffixList = getFileList(folderChoose, nameFiltersAllSuffix);

	QFileInfoList NoSuffixList;

	for (int i = 0; i != AllFilesList.size(); i++)
	{
		QApplication::processEvents();
		if (!(AllSuffixList.contains(AllFilesList.at(i))))
		{
			NoSuffixList.append(AllFilesList.at(i));
			std::string NoSuffixPath_str = AllFilesList.at(i).absoluteFilePath().toStdString();
			std::cout <<"found no suffix file: "<< NoSuffixPath_str << std::endl;
			LogBrowserWindow.printLog(mZlib.str2qstr("<font color='#389fff'>found no suffix file: </font>" + NoSuffixPath_str));
		}
	}
	if (NoSuffixList.size() == 0)
	{
		ui.progressBar_3->setValue(100);
	}
	for (int j = 0; j != NoSuffixList.size(); j++)
	{
		QApplication::processEvents();
		QFileInfoList tempFileList;
		tempFileList.append(NoSuffixList.at(j));// just for progress bar ...
		DCMTK_anonymizeDcm(folderChoose, tempFileList);
		ui.progressBar_3->setValue(100 * (j + 1) / NoSuffixList.size());
	}
	


}

void Anonymizer::slot_btn_chooseFolder()
{
	QString folderChoose = fileDialog->getExistingDirectory();
	if (folderChoose != NULL)
	{
		LogBrowserWindow.ui.logTextBrowser->clear();
		LogBrowserWindow.ui.errorTextBrowser->clear();

		ui.label_3->setText("<font color='#389fff'><b>Processing ...</b></font>");
		ui.label_4->setText("");
		ui.progressBar->setValue(0);
		ui.progressBar_2->setValue(0);
		ui.progressBar_3->setValue(0);

		ui.pushButton->setDisabled(true);
		ui.pushButton_2->setDisabled(true);
		ui.pushButton_3->setDisabled(true);
		ui.pushButton_4->setDisabled(true);

		ui.label_4->setText(folderChoose);
		nameFiltersDcm << "*.dcm";
		QFileInfoList dcmList = getFileList(folderChoose, nameFiltersDcm);
		DCMTK_anonymizeDcm(folderChoose, dcmList);
		nameFiltersZip << "*.zip";
		QFileInfoList zipList = getFileList(folderChoose, nameFiltersZip);
		anonymizeZip(folderChoose, zipList);
		anonymizeNoSuffix(folderChoose);

		ui.label_3->setText("<font color='#389fff'><b>Finished !</b></font>");
		ui.pushButton->setDisabled(false);
		ui.pushButton_2->setDisabled(false);
		ui.pushButton_3->setDisabled(false);
		ui.pushButton_4->setDisabled(false);
	}

    
}

void Anonymizer::slot_btn_chooseFolderForDcm()
{

	QString folderChoose = fileDialog->getExistingDirectory();
	if (folderChoose != NULL)
	{

		LogBrowserWindow.ui.logTextBrowser->clear();
		LogBrowserWindow.ui.errorTextBrowser->clear();

		ui.label_3->setText("<font color='#389fff'><b>Processing ...</b></font>");
		ui.label_4->setText("");
		ui.progressBar->setValue(0);
		ui.progressBar_2->setValue(0);
		ui.progressBar_3->setValue(0);

		ui.pushButton->setDisabled(true);
		ui.pushButton_2->setDisabled(true);
		ui.pushButton_3->setDisabled(true);
		ui.pushButton_4->setDisabled(true);

		ui.label_4->setText(folderChoose);

		nameFiltersDcm << "*.dcm";
		QFileInfoList dcmList = getFileList(folderChoose, nameFiltersDcm);
		DCMTK_anonymizeDcm(folderChoose, dcmList);

		ui.label_3->setText("<font color='#389fff'><b>Finished !</b></font>");
		ui.pushButton->setDisabled(false);
		ui.pushButton_2->setDisabled(false);
		ui.pushButton_3->setDisabled(false);
		ui.pushButton_4->setDisabled(false);
	}


}

void Anonymizer::slot_btn_chooseFolderForZip()
{

	QString folderChoose = fileDialog->getExistingDirectory();
	if (folderChoose != NULL)
	{

		LogBrowserWindow.ui.logTextBrowser->clear();
		LogBrowserWindow.ui.errorTextBrowser->clear();

		ui.label_3->setText("<font color='#389fff'><b>Processing ...</b></font>");
		ui.label_4->setText("");
		ui.progressBar->setValue(0);
		ui.progressBar_2->setValue(0);
		ui.progressBar_3->setValue(0);

		ui.pushButton->setDisabled(true);
		ui.pushButton_2->setDisabled(true);
		ui.pushButton_3->setDisabled(true);
		ui.pushButton_4->setDisabled(true);
		ui.label_4->setText(folderChoose);

		nameFiltersZip << "*.zip";
		QFileInfoList zipList = getFileList(folderChoose, nameFiltersZip);
		anonymizeZip(folderChoose, zipList);


		ui.label_3->setText("<font color='#389fff'><b>Finished !</b></font>");
		ui.pushButton->setDisabled(false);
		ui.pushButton_2->setDisabled(false);
		ui.pushButton_3->setDisabled(false);
		ui.pushButton_4->setDisabled(false);
	}


}

void Anonymizer::slot_btn_chooseFolderForNoSuffix()
{

	QString folderChoose = fileDialog->getExistingDirectory();
	if (folderChoose != NULL)
	{

		LogBrowserWindow.ui.logTextBrowser->clear();
		LogBrowserWindow.ui.errorTextBrowser->clear();

		ui.label_3->setText("Processing...");
		ui.label_3->setStyleSheet("color:#389fff;");
		ui.label_4->setText("");
		ui.progressBar->setValue(0);
		ui.progressBar_2->setValue(0);
		ui.progressBar_3->setValue(0);

		ui.pushButton->setDisabled(true);
		ui.pushButton_2->setDisabled(true);
		ui.pushButton_3->setDisabled(true);
		ui.pushButton_4->setDisabled(true);
		ui.label_4->setText(folderChoose);

		anonymizeNoSuffix(folderChoose);

		ui.label_3->setText("<font color='#389fff'><b>Finished !</b></font>");
		ui.pushButton->setDisabled(false);
		ui.pushButton_2->setDisabled(false);
		ui.pushButton_3->setDisabled(false);
		ui.pushButton_4->setDisabled(false);
	}



}