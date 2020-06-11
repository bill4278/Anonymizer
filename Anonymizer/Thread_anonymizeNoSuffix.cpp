#include "Thread_anonymizeNoSuffix.h"

threadAnonyNoSuffix::threadAnonyNoSuffix(QObject *parent) :QObject(parent)
{
	connect(&m_anonyDcm, &threadAnonyDCM::progressStatus, this, &threadAnonyNoSuffix::getProgressStatus);
	connect(&m_anonyDcm, &threadAnonyDCM::progressError, this, &threadAnonyNoSuffix::getProgressError);
	connect(&m_anonyDcm, &threadAnonyDCM::progressValue, this, &threadAnonyNoSuffix::getProgressValue);
	connect(&m_anonyDcm, &threadAnonyDCM::fatalError, this, &threadAnonyNoSuffix::getFatalError);
	connect(&m_anonyDcm, &threadAnonyDCM::progressFinished, this, &threadAnonyNoSuffix::getProgressFinished);
}
void threadAnonyNoSuffix::getProgressValue(const float value, const int bar_No)
{
	emit progressValue(value, bar_No);
}
void threadAnonyNoSuffix::getProgressError(QString Qstr)
{
	emit progressError(Qstr);
}
void threadAnonyNoSuffix::getProgressStatus(QString Qstr)
{
	emit progressStatus(Qstr);
}
void threadAnonyNoSuffix::getFatalError(QString Qstr)
{
	emit fatalError(Qstr);
}
void threadAnonyNoSuffix::getProgressFinished(bool isFinished)
{
	dcmProgressFinished = isFinished;
}
void threadAnonyNoSuffix::anonymizeNosuffix(QString folderChoose)
{
	nameFiltersAllFile << "*";
	QFileInfoList AllFilesList = m_coP.getFileList(folderChoose, nameFiltersAllFile);

	nameFiltersAllSuffix << "*.*";
	QFileInfoList AllSuffixList = m_coP.getFileList(folderChoose, nameFiltersAllSuffix);

	QFileInfoList NoSuffixList;

	for (int i = 0; i != AllFilesList.size(); i++)
	{
		if (!(AllSuffixList.contains(AllFilesList.at(i))))
		{
			NoSuffixList.append(AllFilesList.at(i));
			std::string NoSuffixPath_str = AllFilesList.at(i).absoluteFilePath().toStdString();
			std::cout << "found no suffix file: " << NoSuffixPath_str << std::endl;
			emit progressStatus(m_coP.str2qstr("<font color='#389fff'>found no suffix file: </font>" + NoSuffixPath_str));
		}
	}
	if (NoSuffixList.size() == 0)
	{
		emit progressValue(100, 3);
	}
	for (int j = 0; j != NoSuffixList.size(); j++)
	{
		QFileInfoList tempFileList;
		tempFileList.append(NoSuffixList.at(j));// just for progress bar ...
		m_anonyDcm.DCMTK_anonymizeDcm_2(folderChoose, tempFileList);
		emit progressValue(100 * (j + 1) / NoSuffixList.size(), 3);
	}
	emit progressFinished(true, 2);
}