#include <iostream>
#include "Anonymizer.h"


//#include "testZip.h"
#include "errno.h"

#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

// https://itk.org/Doxygen/html/Examples_2IO_2DicomImageReadChangeHeaderWrite_8cxx-example.html

Anonymizer::Anonymizer(QWidget *parent)
	: QMainWindow(parent)
{

	ui.setupUi(this);
	setupConnection();

}

void Anonymizer::setupConnection()
{

	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(slot_btn_chooseFolder()));

}

QFileInfoList Anonymizer::getFileList(QString folderChoose , QStringList nameFilters)
{

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
		 
		//LB.printLog(mZlib.str2qstr(strerror(errno)));
	}
}

void Anonymizer::renameFile(const char * oldFilePath, const char * newFileName)
{
	if (std::rename(oldFilePath, newFileName) == -1)
	{
		std::cout << "Error: " << strerror(errno) << std::endl;
		//LB.printLog(mZlib.str2qstr(strerror(errno)));
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
// 			//LB.printLog(mZlib.str2qstr("can not read file: "+ std::string(dcmPath_char)));
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
// 			//LB.printLog(mZlib.str2qstr(std::string(dcmPath_char) + "anonymize failed !!!!!"));
// 		}
// 
// 		std::cout << dcmPath_char<< "  anonymized" << std::endl;
// 		//LB.printLog(mZlib.str2qstr(std::string(dcmPath_char) + "  anonymized"));
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
			//LB.printLog(mZlib.str2qstr("create folder: " + createDir.toStdString()));
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
			//LB.printLog(mZlib.str2qstr("loading: " + std::string(dcmPath_char)));
		}
		DcmDataset * dataset = fileformat.getDataset();
		dataset->putAndInsertString(DCM_PatientName, "Anonymous");
		dataset->putAndInsertString(DCM_PatientID, "	");
		dataset->putAndInsertString(DCM_PatientSex, "	");
		dataset->putAndInsertString(DCM_PatientAge, "	");
		dataset->putAndInsertString(DCM_PatientBirthDate, "	");
		status = fileformat.saveFile(zlib.qstr2str(createDir+"temp.dcm").c_str());
		removeFile(dcmPath_char);
		renameFile(zlib.qstr2str(createDir + "temp.dcm").c_str(), dcmPath_char);
		
		if (status.good())
		{
			std::cout << "anonymized: " << dcmPath_char << std::endl;
			//LB.printLog(mZlib.str2qstr("anonymized: " + std::string(dcmPath_char)));
		}

		
	}
	dir.rmdir(createDir);
}
void Anonymizer::anonymizeZip(QString folderChoose, QFileInfoList zipList)
{
	QString createUnzipDir = folderChoose + "/unzip_temp/";
	QDir dir;
	if (!dir.exists(createUnzipDir))
	{
		bool res = dir.mkdir(createUnzipDir);
		if (res) 
		{
			std::cout << "create folder: " << createUnzipDir.toStdString() << std::endl;
			//LB.printLog(mZlib.str2qstr("create folder: " + createUnzipDir.toStdString()));
		}
	}
	else
	{
		dir.rmdir(createUnzipDir);
	}

	
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
		QApplication::processEvents();
		ui.progressBar_2->setValue(100 * (i + 1) / zipList.size());
		std::string zipPath_str = zipList.at(i).absoluteFilePath().toStdString();
		std::cout << "loading zip file " << zipPath_str << std::endl;
		int succd = mZlib.UnCompress(zipPath_str.c_str(), uncompressPath_char);
		if (succd == 1)
		{
			std::cout << "uncompressed : " << zipPath_str << std::endl;
			//LB.printLog(mZlib.str2qstr("uncompressed: " + std::string(zipPath_str)));
		}
		else
		{
			std::cout << "!!!!!  failed uncompressed : " << zipPath_str << std::endl;
			//LB.printLog(mZlib.str2qstr("!!!!!  failed uncompressed : " + std::string(zipPath_str)));
		}

		if (succd)
		{
			nameFiltersDcm << "*.dcm";
			QFileInfoList zipDcmList = getFileList(uncompressPath_char, nameFiltersDcm);
			DCMTK_anonymizeDcm(uncompressPath_char, zipDcmList);
			
		}

		nameFiltersAllSuffix << "*.*";
		QFileInfoList zipFileList = getFileList(uncompressPath_char, nameFiltersAllSuffix);
		
		mZlib.Compress(zipFileList, compressPath_char, uncompressPath_char);

		QDir d(uncompressPath_char);
		d.setFilter(QDir::Files);
		int j, k = d.count() - 1;
		for (j = 0;j <= k;j++)
		{
			d.remove(d[j]);
		}
		
		std::cout << "removed temp files" << std::endl;
		//LB.printLog(mZlib.str2qstr("removed temp files"));

		removeFile(zipPath_str.c_str());
		renameFile(compressPath_char, zipPath_str.c_str());

	}
	dir.rmdir(uncompressPath_char);// this method only suitable for empty folder
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
		if (!(AllSuffixList.contains(AllFilesList.at(i))))
		{
			NoSuffixList.append(AllFilesList.at(i));
			std::string zipPath_str = AllFilesList.at(i).absoluteFilePath().toStdString();
			std::cout << zipPath_str << std::endl;
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
	
	ui.label_3->setText("Processing...");
	ui.label_4->setText("");
	ui.progressBar->setValue(0);
	ui.progressBar_2->setValue(0);
	ui.progressBar_3->setValue(0);

	QString folderChoose = fileDialog->getExistingDirectory();
	if (folderChoose != NULL)
	{
		ui.pushButton->setDisabled(true);
		ui.label_4->setText(folderChoose);
		nameFiltersDcm << "*.dcm";
		QFileInfoList dcmList = getFileList(folderChoose, nameFiltersDcm);
		DCMTK_anonymizeDcm(folderChoose, dcmList);
		nameFiltersZip << "*.zip";
		QFileInfoList zipList = getFileList(folderChoose, nameFiltersZip);
		anonymizeZip(folderChoose, zipList);
		anonymizeNoSuffix(folderChoose);

		ui.label_3->setText("Finished!");
		ui.pushButton->setDisabled(false);
	}

    
}
