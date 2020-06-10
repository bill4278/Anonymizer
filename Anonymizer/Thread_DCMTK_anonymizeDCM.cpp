#include "Thread_DCMTK_anonymizeDCM.h"

threadAnonyDCM::threadAnonyDCM(QObject *parent) :QObject(parent)
{

}

void threadAnonyDCM::DCMTK_anonymizeDcm_2(const QString folderChoose, const QFileInfoList dcmList)
{

	QString createDir = folderChoose + "/dcm_temp_2/";
	QDir dir;
	if (!dir.exists(createDir))
	{
		bool res = dir.mkdir(createDir);
		if (res)
		{
			std::cout << "create folder: " << createDir.toStdString() << std::endl;
			emit progressStatus(m_coP.str2qstr("<font color = '#389fff'>create folder</font>: " + createDir.toStdString()));
		}
	}
	else
	{
		dir.rmdir(createDir);
	}
	if (dcmList.size() == 0)
	{
		emit progressValue(100, 1);
	}
	for (int i = 0; i != dcmList.size(); i++)
	{
		

		std::string dcmPath_str = dcmList.at(i).absoluteFilePath().toStdString();
		const char* dcmPath_char = dcmPath_str.c_str();

		OFCondition loadStatus;
		OFCondition saveStatus;
		DcmFileFormat fileformat;
		//https://www.cnblogs.com/bayzhang/p/5484321.html
		DcmMetaInfo * metainfo;
		loadStatus = fileformat.loadFile(dcmPath_char);
		if (loadStatus.good())
		{
			std::cout << "loading: " << dcmPath_char << std::endl;
			emit progressStatus(m_coP.str2qstr("<font color = '#389fff'>loading: </font>" + std::string(dcmPath_char)));

			DcmDataset * dataset = fileformat.getDataset();
			dataset->putAndInsertString(DCM_PatientName, "Anonymous222");
			dataset->putAndInsertString(DCM_PatientID, "	");
			dataset->putAndInsertString(DCM_PatientSex, "	");
			dataset->putAndInsertString(DCM_PatientAge, "	");
			dataset->putAndInsertString(DCM_PatientBirthDate, "	");
			saveStatus = fileformat.saveFile(m_coP.qstr2str(createDir + "temp.dcm").c_str());

			int removeStatus = m_coP.removeFile(dcmPath_char);

			int renameStatus = m_coP.renameFile(m_coP.qstr2str(createDir + "temp.dcm").c_str(), dcmPath_char);

			if (saveStatus.good() && (renameStatus==1) && (removeStatus==1))
			{
				std::cout << "anonymized: " << dcmPath_char << std::endl;
				emit progressStatus(m_coP.str2qstr("<font color = '#389fff'><b>anonymized: </b></font>" + std::string(dcmPath_char)));
			}
			else
			{
				emit progressStatus(m_coP.str2qstr("<font color = 'red'><b> error to anonymize: </b></font>") + m_coP.str2qstr(std::string(dcmPath_char)));
				emit progressError(m_coP.str2qstr("<font color = 'red'><b> error to anonymize: </b></font>") + m_coP.str2qstr(std::string(dcmPath_char)));
			}
		}
		else
		{
			emit progressStatus(m_coP.str2qstr("<font color = 'red'><b> error to load: </b></font>") + m_coP.str2qstr(std::string(dcmPath_char)));
			emit progressError(m_coP.str2qstr("<font color = 'red'><b> error to load: </b></font>") + m_coP.str2qstr(std::string(dcmPath_char)));
		}

		emit progressValue(100 * (i + 1) / dcmList.size(), 1);

	}
	QDir d(createDir);
	if (d.removeRecursively())
	{
		std::cout << "removed temp files" << std::endl;
		emit progressStatus(m_coP.str2qstr("<font color = '#389fff'>removed temp files</font>"));
	}
	else
	{
		emit progressStatus(m_coP.str2qstr("<font color = 'red'><b> error to remove folder: </b></font>" + m_coP.qstr2str(createDir)));
		emit progressError(m_coP.str2qstr("<font color = 'red'><b> error to remove folder: </b></font>" + m_coP.qstr2str(createDir)));
		emit fatalError(m_coP.str2qstr("can not remove temporary folder, please terminate and report a bug !"));
	}
	emit progressFinished(true, 0);
}

void threadAnonyDCM::DCMTK_anonymizeDcm(const QString folderChoose)
{
	nameFiltersDcm << ("*.dcm");
	QFileInfoList dcmList = m_coP.getFileList(folderChoose, nameFiltersDcm);

	QString createDir = folderChoose + "/dcm_temp/";
	QDir dir;
	if (!dir.exists(createDir))
	{
		bool res = dir.mkdir(createDir);
		if (res)
		{
			std::cout << "create folder: " << createDir.toStdString() << std::endl;
			emit progressStatus(m_coP.str2qstr("<font color = '#389fff'>create folder</font>: " + createDir.toStdString()));
		}
	}
	else
	{
		dir.rmdir(createDir);
	}
	if (dcmList.size() == 0)
	{
		emit progressValue(100, 1);
	}
	for (int i = 0; i != dcmList.size(); i++)
	{


		std::string dcmPath_str = dcmList.at(i).absoluteFilePath().toStdString();
		const char* dcmPath_char = dcmPath_str.c_str();

		OFCondition loadStatus;
		OFCondition saveStatus;
		DcmFileFormat fileformat;
		//https://www.cnblogs.com/bayzhang/p/5484321.html
		DcmMetaInfo * metainfo;
		loadStatus = fileformat.loadFile(dcmPath_char);
		if (loadStatus.good())
		{
			std::cout << "loading: " << dcmPath_char << std::endl;
			emit progressStatus(m_coP.str2qstr("<font color = '#389fff'>loading: </font>" + std::string(dcmPath_char)));

			DcmDataset * dataset = fileformat.getDataset();
			dataset->putAndInsertString(DCM_PatientName, "Anonymous222");
			dataset->putAndInsertString(DCM_PatientID, "	");
			dataset->putAndInsertString(DCM_PatientSex, "	");
			dataset->putAndInsertString(DCM_PatientAge, "	");
			dataset->putAndInsertString(DCM_PatientBirthDate, "	");
			saveStatus = fileformat.saveFile(m_coP.qstr2str(createDir + "temp.dcm").c_str());

			int removeStatus = m_coP.removeFile(dcmPath_char);

			int renameStatus = m_coP.renameFile(m_coP.qstr2str(createDir + "temp.dcm").c_str(), dcmPath_char);

			if (saveStatus.good() && (renameStatus == 1) && (removeStatus == 1))
			{
				std::cout << "anonymized: " << dcmPath_char << std::endl;
				emit progressStatus(m_coP.str2qstr("<font color = '#389fff'><b>anonymized: </b></font>" + std::string(dcmPath_char)));
			}
			else
			{
				emit progressStatus(m_coP.str2qstr("<font color = 'red'><b> error to anonymize: </b></font>") + m_coP.str2qstr(std::string(dcmPath_char)));
				emit progressError(m_coP.str2qstr("<font color = 'red'><b> error to anonymize: </b></font>") + m_coP.str2qstr(std::string(dcmPath_char)));
			}
		}
		else
		{
			emit progressStatus(m_coP.str2qstr("<font color = 'red'><b> error to load: </b></font>") + m_coP.str2qstr(std::string(dcmPath_char)));
			emit progressError(m_coP.str2qstr("<font color = 'red'><b> error to load: </b></font>") + m_coP.str2qstr(std::string(dcmPath_char)));
		}

		emit progressValue(100 * (i + 1) / dcmList.size(), 1);

	}
	QDir d(createDir);
	if (d.removeRecursively())
	{
		std::cout << "removed temp files" << std::endl;
		emit progressStatus(m_coP.str2qstr("<font color = '#389fff'>removed temp files</font>"));
	}
	else
	{
		emit progressStatus(m_coP.str2qstr("<font color = 'red'><b> error to remove folder: </b></font>" + m_coP.qstr2str(createDir)));
		emit progressError(m_coP.str2qstr("<font color = 'red'><b> error to remove folder: </b></font>" + m_coP.qstr2str(createDir)));
		emit fatalError(m_coP.str2qstr("can not remove temporary folder, please terminate and report a bug !"));
	}
	emit progressFinished(true, 0);
}