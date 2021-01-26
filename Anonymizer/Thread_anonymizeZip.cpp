#include "Thread_anonymizeZip.h"

threadAnonyZip::threadAnonyZip(QObject *parent) :QObject(parent)
{
	connect(&m_anonyDcm, &threadAnonyDCM::progressStatus, this, &threadAnonyZip::getProgressStatus);
	connect(&m_anonyDcm, &threadAnonyDCM::progressError, this, &threadAnonyZip::getProgressError);
	connect(&m_anonyDcm, &threadAnonyDCM::progressValue, this, &threadAnonyZip::getProgressValue);
	connect(&m_anonyDcm, &threadAnonyDCM::fatalError, this, &threadAnonyZip::getFatalError);
	connect(&m_anonyDcm, &threadAnonyDCM::progressFinished, this, &threadAnonyZip::getProgressFinished);
}
void threadAnonyZip::getProgressValue(const float value, const int bar_No)
{
	emit progressValue(value, bar_No);
}
void threadAnonyZip::getProgressError(QString Qstr)
{
	emit progressError(Qstr);
}
void threadAnonyZip::getProgressStatus(QString Qstr)
{
	emit progressStatus(Qstr);
}
void threadAnonyZip::getFatalError(QString Qstr)
{
	emit fatalError(Qstr);
}
void threadAnonyZip::getProgressFinished(bool isFinished)
{
	dcmProgressFinished = isFinished;
}
void threadAnonyZip::anonymizeZip(QString folderChoose, const bool isNeedRemoveInstitute)
{
	nameFiltersZip << ("*.zip");
	QFileInfoList zipList = m_coP.getFileList(folderChoose, nameFiltersZip);
	QString createUnzipDir = folderChoose + "/anonymizer_unzip_temp/";
	QDir dir;


	std::string createUnzipDir_str = m_coP.qstr2str(createUnzipDir);
	const char *uncompressPath_char = createUnzipDir_str.c_str();
	std::cout << uncompressPath_char << std::endl;

	QString createZipDir = folderChoose + "/anonymizer_zip_temp.zip";


	std::string createZipDir_str = m_coP.qstr2str(createZipDir);
	const char *compressPath_char = createZipDir_str.c_str();
	std::cout << compressPath_char << std::endl;



	if (zipList.size() == 0)
	{
		emit progressValue(100, 2);
	}
	for (int i = 0; i != zipList.size(); i++)
	{
		if (!dir.exists(createUnzipDir))
		{
			bool res = dir.mkdir(createUnzipDir);
			if (res)
			{
				std::cout << "create folder: " << createUnzipDir.toStdString() << std::endl;
				emit progressStatus(m_coP.str2qstr("<font color = '#389fff'>create folder: </font>" + createUnzipDir.toStdString()));
			}
		}
		else
		{
			QDir d(createUnzipDir);
			d.removeRecursively();
			bool res = dir.mkdir(createUnzipDir);
			if (res)
			{
				std::cout << "create folder: " << createUnzipDir.toStdString() << std::endl;
				emit progressStatus(m_coP.str2qstr("<font color = '#389fff'>create folder: </font>" + createUnzipDir.toStdString()));
			}
		}

		std::string zipPath_str = zipList.at(i).absoluteFilePath().toStdString();
		std::cout << "loading zip file " << zipPath_str << std::endl;
		int succd = m_zlib.UnCompress(zipPath_str.c_str(), uncompressPath_char);
		if (succd == 1)
		{
			std::cout << "uncompressed : " << zipPath_str << std::endl;
			emit progressStatus(m_coP.str2qstr("<font color = '#389fff'>uncompressed: </font>" + std::string(zipPath_str)));
		}
		else
		{
			std::cout << "!!!!!  failed uncompressed : " << zipPath_str << std::endl;
			emit progressStatus(m_coP.str2qstr("<font color = 'red'><b> error to uncompress : </b></font>" + std::string(zipPath_str)));
			emit progressError(m_coP.str2qstr("<font color = 'red'><b> error to uncompress : </b></font>" + std::string(zipPath_str)));
		}

		QFileInfoList zipDcmList;

		if (succd)
		{
			nameFiltersDcm << "*.dcm";
			zipDcmList = m_coP.getFileList(uncompressPath_char, nameFiltersDcm);
			if (zipDcmList.size() == 0)
			{
				emit progressValue(100, 2);
			}
			else
			{
				m_anonyDcm.DCMTK_anonymizeDcm(uncompressPath_char, isNeedRemoveInstitute);

				nameFiltersAllSuffix << "*.*";
				QFileInfoList zipFileList = m_coP.getFileList(uncompressPath_char, nameFiltersAllSuffix);

				m_zlib.Compress(zipFileList, compressPath_char, uncompressPath_char);
			}
		}
		QDir d(uncompressPath_char);
		if (d.removeRecursively())
		{
			std::cout << "removed temp files" << std::endl;
			emit progressStatus(m_coP.str2qstr("<font color = '#389fff'>removed temp files</font>"));
		}
		else
		{
			emit progressStatus(m_coP.str2qstr("<font color = 'red'><b> error to remove folder: </b></font>" + m_coP.qstr2str(uncompressPath_char)));
			emit progressError(m_coP.str2qstr("<font color = 'red'><b> error to remove folder: </b></font>" + m_coP.qstr2str(uncompressPath_char)));
			emit fatalError(m_coP.str2qstr("can not remove temporary folder, please terminate and report a bug !"));
		}
		if (zipDcmList.size() != 0)
		{
			m_coP.removeFile(zipPath_str.c_str());
			m_coP.renameFile(compressPath_char, zipPath_str.c_str());
		}
		emit progressValue(100 * (i + 1) / zipList.size(), 2);
	}
	
	emit progressFinished(true, 1);
	//dir.rmdir(uncompressPath_char);// this method only suitable for empty folder
}
void threadAnonyZip::anonymizeZip_2(QString folderChoose, QFileInfoList zipList, const bool isNeedRemoveInstitute)
{
	QString createUnzipDir = folderChoose + "/anonymizer_unzip_temp/";
	QDir dir;


	std::string createUnzipDir_str = m_coP.qstr2str(createUnzipDir);
	const char *uncompressPath_char = createUnzipDir_str.c_str();
	std::cout << uncompressPath_char << std::endl;

	QString createZipDir = folderChoose + "/anonymizer_zip_temp.zip";


	std::string createZipDir_str = m_coP.qstr2str(createZipDir);
	const char *compressPath_char = createZipDir_str.c_str();
	std::cout << compressPath_char << std::endl;



	if (zipList.size() == 0)
	{
		emit progressValue(100, 2);
	}
	for (int i = 0; i != zipList.size(); i++)
	{
		if (!dir.exists(createUnzipDir))
		{
			bool res = dir.mkdir(createUnzipDir);
			if (res)
			{
				std::cout << "create folder: " << createUnzipDir.toStdString() << std::endl;
				emit progressStatus(m_coP.str2qstr("<font color = '#389fff'>create folder: </font>" + createUnzipDir.toStdString()));
			}
		}
		else
		{
			QDir d(createUnzipDir);
			d.removeRecursively();
			bool res = dir.mkdir(createUnzipDir);
			if (res)
			{
				std::cout << "create folder: " << createUnzipDir.toStdString() << std::endl;
				emit progressStatus(m_coP.str2qstr("<font color = '#389fff'>create folder: </font>" + createUnzipDir.toStdString()));
			}
		}

		std::string zipPath_str = zipList.at(i).absoluteFilePath().toStdString();
		std::cout << "loading zip file " << zipPath_str << std::endl;
		int succd = m_zlib.UnCompress(zipPath_str.c_str(), uncompressPath_char);
		if (succd == 1)
		{
			std::cout << "uncompressed : " << zipPath_str << std::endl;
			emit progressStatus(m_coP.str2qstr("<font color = '#389fff'>uncompressed: </font>" + std::string(zipPath_str)));
		}
		else
		{
			std::cout << "!!!!!  failed uncompressed : " << zipPath_str << std::endl;
			emit progressStatus(m_coP.str2qstr("<font color = 'red'><b> error to uncompress : </b></font>" + std::string(zipPath_str)));
			emit progressError(m_coP.str2qstr("<font color = 'red'><b> error to uncompress : </b></font>" + std::string(zipPath_str)));
		}

		QFileInfoList zipDcmList;

		if (succd)
		{
			nameFiltersDcm << "*.dcm";
			zipDcmList = m_coP.getFileList(uncompressPath_char, nameFiltersDcm);
			if (zipDcmList.size() == 0)
			{
				emit progressValue(100, 2);
			}
			else
			{
				m_anonyDcm.DCMTK_anonymizeDcm(uncompressPath_char, isNeedRemoveInstitute);

				nameFiltersAllSuffix << "*.*";
				QFileInfoList zipFileList = m_coP.getFileList(uncompressPath_char, nameFiltersAllSuffix);

				m_zlib.Compress(zipFileList, compressPath_char, uncompressPath_char);
			}
		}
		QDir d(uncompressPath_char);
		if (d.removeRecursively())
		{
			std::cout << "removed temp files" << std::endl;
			emit progressStatus(m_coP.str2qstr("<font color = '#389fff'>removed temp files</font>"));
		}
		else
		{
			emit progressStatus(m_coP.str2qstr("<font color = 'red'><b> error to remove folder: </b></font>" + m_coP.qstr2str(uncompressPath_char)));
			emit progressError(m_coP.str2qstr("<font color = 'red'><b> error to remove folder: </b></font>" + m_coP.qstr2str(uncompressPath_char)));
			emit fatalError(m_coP.str2qstr("can not remove temporary folder, please terminate and report a bug !"));
		}
		if (zipDcmList.size() != 0)
		{
			m_coP.removeFile(zipPath_str.c_str());
			m_coP.renameFile(compressPath_char, zipPath_str.c_str());
		}
		emit progressValue(100 * (i + 1) / zipList.size(), 2);
	}

	emit progressFinished(true, 1);
	//dir.rmdir(uncompressPath_char);// this method only suitable for empty folder
}