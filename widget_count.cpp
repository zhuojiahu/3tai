#include "widget_count.h"
#include <QLayout>
#include <QStandardItem>
#include <QTextStream>
#include "glasswaredetectsystem.h"
#include <ActiveQt/QAxObject>
#include <QMap>
extern GlasswareDetectSystem *pMainFrm;
Widget_Count::Widget_Count(QWidget *parent)
	: QWidget(parent),m_TemporaryData(),m_LastData()
{
	for (int i = 0; i < 24;i++)
	{
		for (int j = 0;j<ERRORTYPE_MAX_COUNT;j++)
		{
			m_iErrorHour[i][j] = 0;
		}
	}
	 nLastCheckedNum = 0;
	 nLastFailureNumFromIOcard = 0;
	 for (int i = 0; i < CAMERA_MAX_COUNT;i++)
	 {
		 nLastErrorCamCount[i] = 0;
		 nLastErrorCamRate[i] = 0;
		 nLastErrorTypeInfo[i].Clear();
	 }
	 for (int i = 0; i < ERRORTYPE_MAX_COUNT;i++)
	 {
		 nLastErrorTypeCount[i] = 0;//每个错误类型的错误计数
	 }

	pStackedCount = new QStackedWidget();
	widgetCountSet = new CountSet;
	bIsAutoSetZero = false;

	DHBaseWidget *widget = new DHBaseWidget;
	widget->setWidgetName(tr("Count Table"));

	buttonTurn = new QPushButton; 
	QPixmap iconTurn(":/sysButton/turnImage");
	buttonTurn->setIcon(iconTurn);
	buttonTurn->setFixedSize(iconTurn.size());

	//建立报表页面
	listWidget = new QWidget;
	m_ListCountWidget = new QTableView(listWidget);
	m_ListCountWidget->resizeColumnsToContents();
	m_ListCountWidget->setVisible(false);
	m_ListhourWidget = new QTableView(listWidget);
	m_ListhourWidget->resizeColumnsToContents();
	if (pMainFrm->m_sSystemInfo.iIsCameraCount)
	{
		m_ListCameraWidget = new QTableView(listWidget);
		m_ListCameraWidget->resizeColumnsToContents();
	}

	QVBoxLayout *listLayout = new QVBoxLayout(listWidget);
	listLayout->addWidget(m_ListCountWidget);
	listLayout->addWidget(m_ListhourWidget);
	if (pMainFrm->m_sSystemInfo.iIsCameraCount)
	{
		listLayout->addWidget(m_ListCameraWidget);
	}
	listLayout->setSpacing(5);
	listLayout->setContentsMargins(0, 0, 0, 0);

	//建立查看历史页面
	recordWidget = new QWidget();
	m_WidgetReport = new WidgetReport(recordWidget,pMainFrm->m_sSystemInfo.iCamCount);
	QGridLayout *scrollAreaGridLayout = new QGridLayout(recordWidget);
	scrollAreaGridLayout->addWidget(m_WidgetReport);
	scrollAreaGridLayout->setMargin(0);
	recordWidget->setLayout(scrollAreaGridLayout);
	//直方图报表
	m_widgetCountPlot=new Widget_CountPlot(pMainFrm->m_sSystemInfo.iCamCount);
	

	//建立主页面
	pStackedCount->addWidget(recordWidget);
	//pStackedCount->addWidget(listWidget);
	pStackedCount->addWidget(m_widgetCountPlot);

	QHBoxLayout *nameLayout = new QHBoxLayout;
	nameLayout->addWidget(widget->widgetName);
	nameLayout->addStretch();
	nameLayout->addWidget(buttonTurn);

	QVBoxLayout *countLayout = new QVBoxLayout(widget);
	countLayout->addLayout(nameLayout);
	//countLayout->addWidget(recordWidget);
	countLayout->addWidget(pStackedCount);
	countLayout->setSpacing(5);
	countLayout->setContentsMargins(5, 0, 5, 0);

	widgetCountSet->ui.widget_conut->setWidgetName(tr("Count Info"));
	widgetCountSet->ui.Layout_countTitle->addWidget(widgetCountSet->ui.widget_conut->widgetName);
	widgetCountSet->ui.widget_calendar->setWidgetName(tr("History"));
	widgetCountSet->ui.Layout_calendarTitle->addWidget(widgetCountSet->ui.widget_calendar->widgetName);
	// 	widgetCountSet->ui.widget_calendar->setVisible(false);
	widgetCountSet->ui.widget_saveRecord->setWidgetName(tr("Save Set"));
	widgetCountSet->ui.Layout_saveRecordTitle->addWidget(widgetCountSet->ui.widget_saveRecord->widgetName);
	widgetCountSet->ui.widget_shitf->setWidgetName(tr("Shift Set"));
	widgetCountSet->ui.Layout_shiftTitle->addWidget(widgetCountSet->ui.widget_shitf->widgetName);

	//widgetCountSet->ui.widget_calendar->setVisible(false);

	if(pMainFrm->sLanguage == 0)
	{
		widgetCountSet->ui.calendarWidget->setLocale(QLocale::Chinese);
	}
	

	QHBoxLayout *mainLayout = new QHBoxLayout;
	//mainLayout->addWidget(pStackedCount);
	mainLayout->addWidget(widget);
	mainLayout->addWidget(widgetCountSet);
	setLayout(mainLayout);
	isClear1 = false;
	isClear2 = false;
	isClear3 = false;
	isSaved = false;
	isSavedHourInfo = false;
	bSaveRecord = pMainFrm->m_sSystemInfo.bSaveRecord;
	iSaveRecordInterval = pMainFrm->m_sSystemInfo.iSaveRecordInterval;

	bAutoSetZero = pMainFrm->m_sSystemInfo.bAutoSetZero;
	shift1 = pMainFrm->m_sSystemInfo.shift1;
	shift2 = pMainFrm->m_sSystemInfo.shift2;
	shift3 = pMainFrm->m_sSystemInfo.shift3;
	QTime time = QTime::currentTime();
	int iTime = time.hour()*10000+time.minute()*100+time.second();
	int iShift1 = shift1.hour()*10000 + shift1.minute()*100 + shift1.second();
	int iShift2 = shift2.hour()*10000 + shift2.minute()*100 + shift2.second();
	int iShift3 = shift3.hour()*10000 + shift3.minute()*100 + shift3.second();
	if (iTime>iShift1 && iTime<iShift2)
	{
		isClear1 = true;
	}
	if (iTime>iShift2 && iTime<iShift3)
	{
		isClear2 = true;
	}	
	if (iTime<iShift1 || iTime>iShift3)
	{
		isClear3 = true;
	}
	init();

}

Widget_Count::~Widget_Count()
{
	if ( m_nTimerId != 0 )
		killTimer(m_nTimerId);
	while(m_modelCount->rowCount()>0 )
	{
		QList<QStandardItem*> listItem = m_modelCount->takeRow(0);
		qDeleteAll(listItem);
		listItem.clear();
	}
	while(m_modelHour->rowCount()>0 )
	{
		QList<QStandardItem*> listItem = m_modelHour->takeRow(0);
		qDeleteAll(listItem);
		listItem.clear();
	}
	if (pMainFrm->m_sSystemInfo.iIsCameraCount)
	{
		while(m_modelCamera->rowCount()>0 )
		{
			QList<QStandardItem*> listItem = m_modelCamera->takeRow(0);
			qDeleteAll(listItem);
			listItem.clear();
		}
	}
}

void Widget_Count::init()
{
	m_ListCountWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);	 //禁止编辑
	m_ListCountWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中
	m_ListCountWidget->setSelectionMode(QAbstractItemView::SingleSelection); //单个选中目标
	m_ListCountWidget->verticalHeader()->setVisible(false);					//隐藏行头
	m_ListhourWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);	//禁止编辑
	m_ListhourWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中
	m_ListhourWidget->setSelectionMode(QAbstractItemView::SingleSelection); //单个选中目标
	m_ListhourWidget->verticalHeader()->setVisible(false);					//隐藏行头	//绑定数据model
	if (pMainFrm->m_sSystemInfo.iIsCameraCount)
	{
		m_ListCameraWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);	  //禁止编辑
		m_ListCameraWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中
		m_ListCameraWidget->setSelectionMode(QAbstractItemView::SingleSelection); //单个选中目标
		m_ListCameraWidget->verticalHeader()->setVisible(false);				  //隐藏行头	//绑定数据model
	}

	m_modelCount = new QStandardItemModel(listWidget);
	m_modelHour = new QStandardItemModel(listWidget);
	if (pMainFrm->m_sSystemInfo.iIsCameraCount)
	{
		m_modelCamera = new QStandardItemModel(listWidget);
	}
	//插入列表头
	QStringList headerList;
	headerList<<QObject::tr("Type");
	headerList<<QObject::tr("Count");
	for (int i=0;i<pMainFrm->m_sSystemInfo.iCamCount;i++)
	{
		headerList <<QObject::tr("Camera%1").arg(i+1);
	}
	m_modelCount->setHorizontalHeaderLabels(headerList);

	QStringList headerListhour; 
	headerListhour<<QObject::tr("Time");
	headerListhour<<QObject::tr("Product Number");
	headerListhour<<QObject::tr("Reject Number");

	for (int i=1;i<=pMainFrm->m_sErrorInfo.m_iErrorTypeCount;i++)
	{
		headerListhour <<pMainFrm->m_sErrorInfo.m_vstrErrorType.at(i);
	}
	headerListhour<<QObject::tr("Count");
	m_modelHour->setHorizontalHeaderLabels(headerListhour);
	if (pMainFrm->m_sSystemInfo.iIsCameraCount)
	{
		QStringList headerListCamera; 
		headerListCamera<<QObject::tr("Time");
		headerListCamera<<QObject::tr("Body Number");
		headerListCamera<<QObject::tr("Mouth Number");
		headerListCamera<<QObject::tr("Buttom Number");
		//headerListCamera<<QObject::tr("ButtomStress Number");

		for (int i=0;i<pMainFrm->m_sSystemInfo.iCamCount;i++)
		{
			headerListCamera <<QObject::tr("Camera%1").arg(i+1);
		}

		m_modelCamera->setHorizontalHeaderLabels(headerListCamera);
	}

	m_ListCountWidget->setModel(m_modelCount);//利用setModel()方法将数据模型与QTableView绑定
	m_ListhourWidget->setModel(m_modelHour);
	if (pMainFrm->m_sSystemInfo.iIsCameraCount)
	{
		m_ListCameraWidget->setModel(m_modelCamera);
	}

	for (int j=0;j<=pMainFrm->m_sErrorInfo.m_iErrorTypeCount;j++)
	{
		iIfShown[j] = 0;
		iListOrder[j] = -1;
		//m_iErrorTypeCount[j] = 0;

	}
	for (int i=0;i<pMainFrm->m_sSystemInfo.iCamCount;i++)
	{
		m_cErrorTypeInfo[i].m_iErrorTypeCount =  pMainFrm->m_sRunningInfo.m_cErrorTypeInfo[i].m_iErrorTypeCount;
		//m_iErrorCamCount[i] = 0;// 每个相机的错误计数
		m_iLastErrorCamCount[i] = 0;// 每个相机的错误计数
	}
	QList<QStandardItem*> itemTotal;
	QStandardItem *tempItem =new QStandardItem(tr("Count"));
	itemTotal<<tempItem;
	for (int i=0;i<pMainFrm->m_sSystemInfo.iCamCount;i++)
	{
		pMainFrm->m_sRunningInfo.m_mutexRunningInfo.lock();
		QStandardItem *itemCount = new QStandardItem(QString::number(pMainFrm->m_sRunningInfo.m_iErrorCamCount[i]));
		pMainFrm->m_sRunningInfo.m_mutexRunningInfo.unlock();

		itemTotal<<itemCount;
	}
	m_modelCount->insertRow(m_modelCount->rowCount(),itemTotal);

	initLastData();

	InsertLastCountInfo();
	timerSaveHourList = new QTimer(this);
	timerSaveHourList->setInterval(60000);//每分钟触发一次
	connect(timerSaveHourList, SIGNAL(timeout()), this, SLOT(slots_SaveCountHourInfoBytime()));   
	timerSaveHourList->start();  	//启动定时器    


	if (bAutoSetZero)
	{
		connect(timerSaveHourList, SIGNAL(timeout()), this, SLOT(slots_timerClearCountInfo()));   
	}
	else
	{
		disconnect(timerSaveHourList, SIGNAL(timeout()), this, SLOT(slots_timerClearCountInfo()));   

	}
	connect(widgetCountSet->ui.pushButton_openRecord,SIGNAL(clicked()),this,SLOT(slots_OpenRecord()));
	connect(widgetCountSet->ui.pushButton_deleteRecord,SIGNAL(clicked()),this,SLOT(slots_DeleteRecord()));

	connect(widgetCountSet->ui.pushButton_save,SIGNAL(clicked()),this,SLOT(slots_SaveCountInfo()));
	connect(widgetCountSet->ui.pushButton_save,SIGNAL(clicked()),this,SLOT(slots_InsertCountHourInfo()));
	connect(widgetCountSet->ui.pushButton_save,SIGNAL(clicked()),this,SLOT(slots_InsertCountCameraInfo()));
	connect(widgetCountSet->ui.pushButton_save,SIGNAL(clicked()),this,SLOT(slots_SaveALLCountInfo()));
	connect(widgetCountSet->ui.pushButton_clear,SIGNAL(clicked()),this,SLOT(slots_ClearCountInfo()));

	connect(widgetCountSet->ui.pushButton_saveRecordOK,SIGNAL(clicked()),this,SLOT(slots_saveRecordOK()));
	connect(widgetCountSet->ui.pushButton_saveRecordCancel,SIGNAL(clicked()),this,SLOT(slots_saveRecordCancel()));

	connect(widgetCountSet->ui.pushButton_shiftOK,SIGNAL(clicked()),this,SLOT(slots_shiftOK()));
	connect(widgetCountSet->ui.pushButton_shiftCancel,SIGNAL(clicked()),this,SLOT(slots_shiftCancel()));
	connect(buttonTurn,SIGNAL(clicked()),this,SLOT(slots_turn()));
}

void Widget_Count::initLastData()
{
	QDateTime dateTime = QDateTime::currentDateTime();
	QDate lastDate =  pMainFrm->LastTime.date();
	QDate CurrentDate = dateTime.date();
	int LastHour = pMainFrm->LastTime.time().hour();
	int CureentHour = dateTime.time().hour();
	if (lastDate == CurrentDate && LastHour == CureentHour && dateTime > pMainFrm->LastTime)
	{
		//m_LastData.m_AllCount = pMainFrm->LastTimeData.m_AllCount;
		//m_LastData.m_FailCount = pMainFrm->LastTimeData.m_FailCount;
		for (int i=1;i<ERRORTYPE_MAX_COUNT;i++)
		{
			//m_LastData.m_CameraTypeCount[i] = pMainFrm->LastTimeData.m_CameraTypeCount[i];
		}
		for (int i=1;i<CAMERA_MAX_COUNT;i++)
		{
			for(int j=1;j<50;j++)
			{
				//m_LastData.m_TypeCount[i][j] = pMainFrm->LastTimeData.m_TypeCount[i][j];
			}
		}
	}
	else
	{
		//m_LastData.m_AllCount = pMainFrm->m_sRunningInfo.m_checkedNum;
		//m_LastData.m_FailCount = pMainFrm->m_sRunningInfo.m_failureNumFromIOcard;
		for (int i=1;i<ERRORTYPE_MAX_COUNT;i++)
		{
			//m_LastData.m_CameraTypeCount[i] = pMainFrm->m_sRunningInfo.m_iErrorTypeCount[i];
		}
		for (int i=1;i<CAMERA_MAX_COUNT;i++)
		{
			for(int j=1;j<50;j++)
			{
				//m_LastData.m_TypeCount[i][j] = pMainFrm->m_sRunningInfo.m_cErrorTypeInfo[i-1].iErrorCountByType[j];
			}
		}
	}
}

void Widget_Count::upDateListInfo()
{
	pMainFrm->m_sRunningInfo.m_mutexRunningInfo.lock();
	for (int j=0;j<=pMainFrm->m_sErrorInfo.m_iErrorTypeCount;j++)
	{
		m_iErrorTypeCount[j] = pMainFrm->m_sRunningInfo.m_iErrorTypeCount[j];
		for (int i=0;i<pMainFrm->m_sSystemInfo.iCamCount;i++) 
		{
			m_cErrorTypeInfo[i].iErrorCountByType[j] = pMainFrm->m_sRunningInfo.m_cErrorTypeInfo[i].iErrorCountByType[j];
		}
	}
	for (int i=0;i<pMainFrm->m_sSystemInfo.iCamCount;i++)
	{
		m_iErrorCamCount[i] = pMainFrm->m_sRunningInfo.m_iErrorCamCount[i];
	}

	pMainFrm->m_sRunningInfo.m_mutexRunningInfo.unlock();

}
bool Widget_Count::slots_intoWidget()
{
	upDateListInfo();
	//更新按缺陷的报表
	while(m_modelCount->rowCount()>0 )
	{
		QList<QStandardItem*> listItem = m_modelCount->takeRow(0);
		qDeleteAll(listItem);
		listItem.clear();
	}
	for (int j=1;j<=pMainFrm->m_sErrorInfo.m_iErrorTypeCount;j++)
	{
		QList<QStandardItem*> item;
		iIfShown[j] = 0;
		for (int i=0;i<pMainFrm->m_sSystemInfo.iCamCount;i++)
		{
			if (0 != m_cErrorTypeInfo[i].iErrorCountByType[j])
			{
				iIfShown[j] = 1;
			}
		}
		if (1 == iIfShown[j])
		{

			QStandardItem *item1 = new QStandardItem(pMainFrm->m_sErrorInfo.m_vstrErrorType.at(j));
			item<<item1;//pMainFrm->m_sErrorInfo.m_vstrErrorType.at(j);
			QStandardItem *item3 = new QStandardItem(QString::number(m_iErrorTypeCount[j]));
			item<<item3;
			for (int i=0;i<pMainFrm->m_sSystemInfo.iCamCount;i++)
			{
				QStandardItem *item2 = new QStandardItem(QString::number(m_cErrorTypeInfo[i].iErrorCountByType[j]));//pMainFrm->m_sRunningInfo.m_cErrorTypeInfo[i].m_iErrorTypeCount[j]));
				item<<item2;
			}
			m_modelCount->insertRow(m_modelCount->rowCount(),item);
		}
	}

	QList<QStandardItem*> item2;
	QStandardItem *tempItem =new QStandardItem(tr("Count"));
	item2<<tempItem;
	int sum = 0;
	for (int i=0;i<pMainFrm->m_sSystemInfo.iCamCount;i++)
	{
		sum += m_iErrorCamCount[i];
	}
	QStandardItem *itemSum = new QStandardItem(QString::number(sum));
	item2<<itemSum;
	for (int i=0;i<pMainFrm->m_sSystemInfo.iCamCount;i++)
	{
		QStandardItem *itemCount = new QStandardItem(QString::number(m_iErrorCamCount[i]));
		item2<<itemCount;
	}

	int iRow = m_modelCount->rowCount();
	m_modelCount->insertRow(m_modelCount->rowCount(),item2);
	slots_saveRecordCancel();
	slots_shiftCancel();
	return true;
}
bool Widget_Count::leaveWidget()
{
	return true;
}
void Widget_Count::slots_OpenRecord()
{
	QDate dateSelecte = widgetCountSet->ui.calendarWidget->selectedDate();
	// 	QString path = pMainFrm->m_sConfigInfo.m_strAppPath + "CountInfo/" + dateSelecte.year() + "-" + dateSelecte.month() + "-" + dateSelecte.day() + ".txt";
	//QString strFileName = pMainFrm->m_sConfigInfo.m_strAppPath + "CountInfo/" + dateSelecte.toString(Qt::ISODate) + ".txt";
	QString strFileName;
	if(0==widgetCountSet->ui.comboBox->currentIndex())
	{
		strFileName = pMainFrm->m_sConfigInfo.m_strAppPath + "CountInfo/HalfCount/" + dateSelecte.toString(Qt::ISODate) + ".txt";
	}else{
		strFileName = pMainFrm->m_sConfigInfo.m_strAppPath + "CountInfo/AllCount/" + dateSelecte.toString(Qt::ISODate) + ".txt";
	}
	if(!QFile::exists(strFileName))
	{
		QMessageBox::information(this,tr("Information"),tr("No record in select date!"));
		return;
	}

	openInNotePad(strFileName);

	QFile readFile(strFileName);

	if (!readFile.open(QFile::ReadOnly | QIODevice::Text))
	{
		pMainFrm->Logfile.write(tr("Fail in Open Record File " + dateSelecte.toString("Qt::ISODate")), AbnormityLog);
		return;
	}

	QTextStream readStream(&readFile);
	//	QTextStream writeStream(&writeFile);
	while (!readStream.atEnd())
	{
		QString line(readStream.readLine());
	}
	readFile.close();

}
DWORD Widget_Count::GetProcessIdFromName(const char*processName)    
{
	PROCESSENTRY32 pe;
	DWORD id = 0;

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	pe.dwSize = sizeof(PROCESSENTRY32);
	if( !Process32First(hSnapshot,&pe) )
		return 0;
	char pname[300];
	do
	{
		pe.dwSize = sizeof(PROCESSENTRY32);
		if( Process32Next(hSnapshot,&pe)==FALSE )
			break;
		//把WCHAR*类型转换为const char*类型
		sprintf_s(pname,"%ws",pe.szExeFile,260);
		//比较两个字符串，如果找到了要找的进程
		if(strcmp(pname,processName) == 0)
		{
			id = pe.th32ProcessID;
			break;
		}

	} while(1);

	CloseHandle(hSnapshot);

	return id;
}
void Widget_Count::openInNotePad(QString str)
{
	DWORD pid = GetProcessIdFromName("notepad.exe");
	//获取进程的最大权限
	if(pid!=0)
	{
		HANDLE token = OpenProcess(PROCESS_ALL_ACCESS,FALSE,pid);
		//关闭进程
		TerminateProcess(token, 0);
	}
	ShellExecuteA(NULL,"open",(LPCSTR)str.toLocal8Bit(),NULL,NULL,SW_SHOW);
}

void Widget_Count::slots_DeleteRecord()
{
	QDate dateSelecte = widgetCountSet->ui.calendarWidget->selectedDate();
	// 	QString strFileName = QString(pMainFrm->m_sConfigInfo.m_strAppPath + "CountInfo/" + dateSelecte.year() + "-" + dateSelecte.month() + "-" + dateSelecte.day() + ".txt");
	QString strDirPath = pMainFrm->m_sConfigInfo.m_strAppPath + "CountInfo/HalfCount/";
	QString strFileName;
	if(0==widgetCountSet->ui.comboBox->currentIndex())
	{
		strFileName = pMainFrm->m_sConfigInfo.m_strAppPath + "CountInfo/HalfCount/" + dateSelecte.toString(Qt::ISODate) + ".txt";
	}else{
		strFileName = pMainFrm->m_sConfigInfo.m_strAppPath + "CountInfo/AllCount/" + dateSelecte.toString(Qt::ISODate) + ".txt";
	}
	if(!QFile::exists(strFileName))
	{
		QMessageBox::information(this,tr("Information"),tr("No record in select date!"));
	}
	QDir dir(strDirPath);

	if (!dir.remove(strFileName))
	{
		QMessageBox::information(this,tr("Information"),tr("Deleting [%1] fail!").arg(strFileName));
		return;
	}

}
bool Widget_Count::slots_ClearCountInfo()
{
	if (QMessageBox::No == QMessageBox::question(this,tr("Exit"),
		tr("Are you sure to clear?"),
		QMessageBox::Yes | QMessageBox::No))	
	{
		return 0 ;
	}
	ClearAllData(1);
	return true;
}
bool Widget_Count::slots_SaveCountHourInfoBytime()
{
	QTime time = QTime::currentTime();
	if (0 == time.minute())
	{
		if (!isSavedHourInfo)
		{
			//			pMainFrm->Logfile.write(tr("AutoFreshHourInfo!"),OperationLog,0);
			slots_InsertCountHourInfo();
			if (pMainFrm->m_sSystemInfo.iIsCameraCount)
			{
				slots_InsertCountCameraInfo();
			}
			isSavedHourInfo = true;
		}
	}
	else
	{
		//确认是创建成功
		QString strFileName;
		
		strFileName = pMainFrm->m_sConfigInfo.m_strAppPath + "CountInfo/HalfCount/";
		
		//QString strFileName = pMainFrm->m_sConfigInfo.m_strAppPath + "CountInfo/";
		QDir temp;// = new QDir;
		bool exist = temp.exists(strFileName);
		if(!exist)
		{
			pMainFrm->Logfile.write(tr("Path not Exist,Create it!"),OperationLog,0);
			bool ok = temp.mkdir(strFileName);
		}


		bool ret = false;
		QDate date = QDate::currentDate();
		strFileName = strFileName +	date.toString(Qt::ISODate) + ".txt";
		//strFileName = strFileName +	date.toString(Qt::ISODate) + ".xls";

		if(!QFile::exists(strFileName))
		{
			pMainFrm->Logfile.write(tr("File not Exist,Create it!"),OperationLog,0);

			QFile createFile(strFileName);
			if(!createFile.open(QFile::WriteOnly | QIODevice::Text))
			{
				pMainFrm->Logfile.write(tr("Create Count File Failure!"),OperationLog,0);
				return ret;
			}
			//bIsEmptyFile = true;
			createFile.close();
		}
		else
		{
			isSavedHourInfo = false;
		}
	}
	
	//pMainFrm->Logfile.write(tr("AutoSaveCountInfo!"),OperationLog,0);
	//slots_SaveCountInfo();

	if (30 == iSaveRecordInterval)
	{
		if ( 30 == time.minute()||0 == time.minute() )
		{
			if (!isSaved)
			{
				SaveToDatabase();
				//				pMainFrm->Logfile.write(tr("AutoSaveCountInfo!"),OperationLog,0);
				slots_SaveCountInfo();
				isSaved = true;
			}
			//			timerSaveRecord->setInterval(30*60000);//半小时触发一次
			// 			return true;
		}
		else
		{
			isSaved = false;
			return false;
		}
	}
	else if (60 == iSaveRecordInterval)
	{
		if (0 == time.minute())
		{
			if (!isSaved)
			{
				SaveToDatabase();
				//pMainFrm->Logfile.write(tr("AutoSaveCountInfo!"),OperationLog,0);
				slots_SaveCountInfo();
				isSaved = true;
			}
			//timerSaveRecord->setInterval(60*60000);//1小时触发一次
		}
		else
		{
			isSaved = false;
			return false;
		}
	}
	else
	{
		return false;
	}
	//	slots_SaveCountInfo();
	return true;

}

void Widget_Count::SaveToDatabase()
{
	QDateTime dateTime = QDateTime::currentDateTime();
	m_TemporaryData.m_AllCount += pMainFrm->m_sRunningInfo.m_checkedNum - m_LastData.m_AllCount;
	m_TemporaryData.m_FailCount += pMainFrm->m_sRunningInfo.m_failureNumFromIOcard - m_LastData.m_FailCount;
	for (int i=1;i<ERRORTYPE_MAX_COUNT;i++)
	{
		m_TemporaryData.m_CameraTypeCount[i] += pMainFrm->m_sRunningInfo.m_iErrorTypeCount[i] - m_LastData.m_CameraTypeCount[i];
	}
	for (int i=1;i<CAMERA_MAX_COUNT;i++)
	{
		for(int j=1;j<50;j++)
		{
			m_TemporaryData.m_TypeCount[i][j] += pMainFrm->m_sRunningInfo.m_cErrorTypeInfo[i-1].iErrorCountByType[j] - m_LastData.m_TypeCount[i][j];
		}
	}
	//所有缺陷总数 小于 剔废总数 不合格数取缺陷总数
	int sum=0;
	for (int i=1;i<ERRORTYPE_MAX_COUNT;i++)
	{
		sum += m_TemporaryData.m_CameraTypeCount[i];
	}
	m_TemporaryData.m_FailCount  = sum < m_TemporaryData.m_FailCount ? sum : m_TemporaryData.m_FailCount;
	m_WidgetReport->m_database->insert(dateTime.toString("yyyyMMddhhmm"),m_TemporaryData);
	m_TemporaryData.TemporaryDataClear();

	//记录数据
	m_LastData.m_AllCount =  pMainFrm->m_sRunningInfo.m_checkedNum ;
	m_LastData.m_FailCount = pMainFrm->m_sRunningInfo.m_failureNumFromIOcard;
	for (int i=1;i<ERRORTYPE_MAX_COUNT;i++)
	{
		m_LastData.m_CameraTypeCount[i] = pMainFrm->m_sRunningInfo.m_iErrorTypeCount[i];
	}
	for (int i=1;i<CAMERA_MAX_COUNT;i++)
	{
		for(int j=1;j<50;j++)
		{
			m_LastData.m_TypeCount[i][j] = pMainFrm->m_sRunningInfo.m_cErrorTypeInfo[i-1].iErrorCountByType[j];
		}
	}
	//保存整点数据
	QSettings LoadLastData(pMainFrm->SaveDataPath,QSettings::IniFormat);
	LoadLastData.setIniCodec(QTextCodec::codecForName("GBK"));
	QString strSession;
	strSession = QString("LastTimeDate/LastTime");
	LoadLastData.setValue(strSession,dateTime.toString("yyyy-MM-dd hh:mm"));
	strSession = QString("LastTimeDate/Checknumber");
	LoadLastData.setValue(strSession,m_LastData.m_AllCount);
	strSession = QString("LastTimeDate/Failurenumber");
	LoadLastData.setValue(strSession,m_LastData.m_FailCount);
	for (int i=1;i<ERRORTYPE_MAX_COUNT;i++)
	{
		strSession = QString("LastTimeDate/ErrorType_%1_count").arg(i);
		LoadLastData.setValue(strSession,m_LastData.m_CameraTypeCount[i]);
	}
	for (int i=1;i<CAMERA_MAX_COUNT;i++)
	{
		for(int j=1;j<ERRORTYPE_MAX_COUNT;j++)
		{
			strSession = QString("LastTimeDate/Camera%1_ErrorType%2").arg(i).arg(j);
			LoadLastData.setValue(strSession,m_LastData.m_TypeCount[i][j]);
		}
	}

}

bool Widget_Count::slots_SaveCountInfo()
{
	pMainFrm->Logfile.write(tr("SaveCountInfo!"),OperationLog,0);
	upDateListInfo();

	bool bIsEmptyFile = false;
	QString strFileName;
	
	strFileName = pMainFrm->m_sConfigInfo.m_strAppPath + "CountInfo/HalfCount/";
	
	
	QDir *temp = new QDir;
	bool exist = temp->exists(strFileName);
	if(!exist)
	{
		bool ok = temp->mkdir(strFileName);
	}

	bool ret = false;
	QDate date = QDate::currentDate();
	strFileName = strFileName +	date.toString(Qt::ISODate) + ".txt";
	//strFileName = strFileName +	date.toString(Qt::ISODate) + ".xls";

	if(!QFile::exists(strFileName))
	{
		QFile createFile(strFileName);
		if(!createFile.open(QFile::WriteOnly | QIODevice::Text))
		{
			pMainFrm->Logfile.write(tr("Create Count File Failure!"),OperationLog,0);
			return ret;
		}

		bIsEmptyFile = true;
		createFile.close();
	}

	QFile readFile(strFileName);

	if (!readFile.open(QFile::Append | QIODevice::Text))
	{
		ret = false;
		pMainFrm->Logfile.write(tr("Read Count File Failure!"),OperationLog,0);
		return ret;
	}

	QFile writeFile(strFileName);

	//读入流和写入流
	//写入流文件在最后才WriteOnly打开
	//	QTextStream readStream(&readFile);
	QTextStream writeStream(&writeFile);

	// 	while (!readStream.atEnd())
	// 	{
	// 		QString line(readStream.readLine());
	// 		writeStream<<line<<"\n";
	// 	}
	// 	readFile.close();

	if(true)
	{
		if (!bIsEmptyFile)
		{
			writeStream<<"\n";

		}

		QTime time = QTime::currentTime();
		writeStream<<QString(tr("Time:  %1:%2:%3")).arg(time.hour()).arg(time.minute()).arg(time.second())<<"\t";	//保存数据时间
		writeStream<<QString(tr("Product Number:  %1")).arg(pMainFrm->m_sRunningInfo.m_checkedNum)<<"\t";			//检测总数
		writeStream<<QString(tr("Reject Number:  %1")).arg(pMainFrm->m_sRunningInfo.m_failureNumFromIOcard)<<"\t";//不合格数 从IO卡反馈
		double dFailurRate=0.0;
		if (0 != pMainFrm->m_sRunningInfo.m_checkedNum  )
		{
			dFailurRate = 1.0*pMainFrm->m_sRunningInfo.m_failureNumFromIOcard/pMainFrm->m_sRunningInfo.m_checkedNum*100;//不合格率
		}
		writeStream<<QString(tr("Reject Rate:  %1%")).arg(dFailurRate)<<"\n";

		//保存上一次数据 Lastdata.ini
		QSettings LoadLastData(pMainFrm->SaveDataPath,QSettings::IniFormat);
		LoadLastData.setIniCodec(QTextCodec::codecForName("GBK"));
		QString strSession;
		strSession = QString("HeadCount/Checknumber");
		LoadLastData.setValue(strSession,pMainFrm->m_sRunningInfo.m_checkedNum);
		strSession = QString("HeadCount/Failurenumber");
		LoadLastData.setValue(strSession,pMainFrm->m_sRunningInfo.m_failureNumFromIOcard);

		strSession = QString("HeadCount/minrate");
		LoadLastData.setValue(strSession,pMainFrm->test_widget->MinRate);

		strSession = QString("HeadCount/maxrate");
		LoadLastData.setValue(strSession,pMainFrm->test_widget->MaxRate);
		
		int iTempbody = 0, iTempfinish = 0, iTempBottom=0;
		for(int i=0;i<=pMainFrm->m_sSystemInfo.iCamCount;i++)
		{
			if(0 == pMainFrm->m_sCarvedCamInfo[i].m_iImageType)
			{
				iTempbody += pMainFrm->m_sRunningInfo.m_iErrorCamCount[i]; 
			}
			else if(1 == pMainFrm->m_sCarvedCamInfo[i].m_iImageType)
			{
				iTempfinish += pMainFrm->m_sRunningInfo.m_iErrorCamCount[i]; 
			}		
			else if(2 == pMainFrm->m_sCarvedCamInfo[i].m_iImageType)
			{
				iTempBottom += pMainFrm->m_sRunningInfo.m_iErrorCamCount[i]; 
			}			
		}
		writeStream<<QString(tr("Reject Number of body:  %1")).arg(iTempbody)<<"\t";		//瓶身剔废
		writeStream<<QString(tr("Reject Number of finish:  %1")).arg(iTempfinish)<<"\t";	//瓶口剔废
		writeStream<<QString(tr("Reject Number of bottom:  %1")).arg(iTempBottom)<<"\n";	//瓶底剔废
		for(int i=1;i<=pMainFrm->m_sErrorInfo.m_iErrorTypeCount;i++)
		{
			writeStream<<pMainFrm->m_sErrorInfo.m_vstrErrorType.at(i) + ":" + QString::number(pMainFrm->m_sRunningInfo.m_iErrorTypeCount[i])<<"\t";

		}
		writeStream<<"\n";
		writeStream<<tr("The statistical results")<<"\t";
		writeStream<<tr("Count")<<"\t";

		for (int i = 0;i<pMainFrm->m_sSystemInfo.iCamCount;i++)
		{
			if (i<9)
			{
				writeStream<<tr("Camera0%1").arg(i+1)<<"\t";
			}
			else
			{
				writeStream<<tr("Camera%1").arg(i+1)<<"\t";
			}
		}
		writeStream<<"\n";
		for (int j = 1;j<=pMainFrm->m_sErrorInfo.m_iErrorTypeCount;j++)
		{
			for (int i=0;i<pMainFrm->m_sSystemInfo.iCamCount;i++)
			{
				if (0 != m_cErrorTypeInfo[i].iErrorCountByType[j])
				{
					iIfShown[j] = 1;
				}

			}
			if (0 != iIfShown[j])
			{
				int k = pMainFrm->m_sErrorInfo.m_vstrErrorType.size();

				QString tempString = pMainFrm->m_sErrorInfo.m_vstrErrorType.at(j);
				writeStream<<tempString;
				if (tempString.length()<4)
				{
					// 					writeStream<<"\t";
					writeStream<<"\t"<<"\t";
				}
				else// if (tempString.length()>=4&&tempString.length()<8)
				{
					writeStream<<"\t";
				}

				writeStream<<QString::number(m_iErrorTypeCount[j])<<"\t";

				for (int i = 0;i<pMainFrm->m_sSystemInfo.iCamCount;i++)
				{
					writeStream<<QString::number(m_cErrorTypeInfo[i].iErrorCountByType[j])<<"\t";

					strSession = QString("DefaultTypeCount/EveryRow%1").arg(i);
					LoadLastData.setValue(strSession,i);
					strSession = QString("DefaultTypeCount/EveryLine%1").arg(j);
					LoadLastData.setValue(strSession,j);
					strSession = QString("DefaultTypeCount/EveryNumber%1_%2").arg(i).arg(j);
					LoadLastData.setValue(strSession,m_cErrorTypeInfo[i].iErrorCountByType[j]);
				}
				writeStream<<"\n";
			}
		}
		//writeStream<<tr("Count:")<<"\t";
		//writeStream<<tr("Count:")<<"\t\t";
		/*int sum = 0;
		for (int i=0;i<pMainFrm->m_sSystemInfo.iCamCount;i++)
		{
			sum += m_iErrorCamCount[i];
		}*/
		//writeStream<<"\t";
		//writeStream<<QString::number(sum)<<"\t";

		/*for (int i = 0;i<pMainFrm->m_sSystemInfo.iCamCount;i++)
		{
			writeStream<<QString::number(m_iErrorCamCount[i])<<"\t";
		}*/
		ret = true;

		writeStream<<"\n";

	}
	//
	if (!writeFile.open(QFile::Append | QIODevice::Text))
	{
		ret = false;
		pMainFrm->Logfile.write(tr("Write Count File Failure!"),OperationLog,0);
		return ret;
	}
	writeStream.flush();//写入流到文件

	writeFile.close();
	delete temp;
	pMainFrm->Logfile.write(tr("SaveCountInfoOver!"),OperationLog,0);
	if (pMainFrm->m_sSystemInfo.iIsSaveCountInfoByTime)
	{
		SaveCountInfoByTime();
	}

	return ret;
}
bool Widget_Count::SaveCountInfoByTime()
{
	pMainFrm->Logfile.write(tr("SaveCountInfo!"),OperationLog,0);
	upDateListInfo();

	bool bIsEmptyFile = false;
	QString strFileName = pMainFrm->m_sConfigInfo.m_strAppPath + "CountInfoByTime/";
	QDir *temp = new QDir;
	bool exist = temp->exists(strFileName);
	if(!exist)
	{
		bool ok = temp->mkdir(strFileName);
	}

	bool ret = false;
	QDate date = QDate::currentDate();
	strFileName = strFileName +	date.toString(Qt::ISODate) + ".txt";
	//strFileName = strFileName +	date.toString(Qt::ISODate) + ".xls";

	if(!QFile::exists(strFileName))
	{
		QFile createFile(strFileName);
		if(!createFile.open(QFile::WriteOnly | QIODevice::Text))
		{
			pMainFrm->Logfile.write(tr("Create Count File Failure!"),OperationLog,0);
			return ret;
		}

		bIsEmptyFile = true;
		createFile.close();
	}

	QFile readFile(strFileName);

	if (!readFile.open(QFile::Append | QIODevice::Text))
	{
		ret = false;
		pMainFrm->Logfile.write(tr("Read Count File Failure!"),OperationLog,0);
		return ret;
	}

	QFile writeFile(strFileName);

	//读入流和写入流
	//写入流文件在最后才WriteOnly打开
	QTextStream writeStream(&writeFile);

	//计算时间段内的计数
	nTempCheckedNum = pMainFrm->m_sRunningInfo.m_checkedNum - nLastCheckedNum;
	nTempFailureNumFromIOcard = pMainFrm->m_sRunningInfo.m_failureNumFromIOcard - nLastFailureNumFromIOcard;
	for (int i = 0; i < CAMERA_MAX_COUNT;i++)
	{
		nTempErrorCamCount[i] = m_iErrorCamCount[i] - nLastErrorCamCount[i];
		for (int j = 0; j<ERRORTYPE_MAX_COUNT; j++)
		{
			nTempErrorTypeInfo[i].m_iErrorTypeCount = m_cErrorTypeInfo[i].m_iErrorTypeCount;
			nTempErrorTypeInfo[i].iErrorCountByType[j] = m_cErrorTypeInfo[i].iErrorCountByType[j] - nLastErrorTypeInfo[i].iErrorCountByType[j];
		}

	}
	for (int i = 0; i < ERRORTYPE_MAX_COUNT;i++)
	{
		nTempErrorTypeCount[i]  = m_iErrorTypeCount[i] - nLastErrorTypeCount[i];//每个错误类型的错误计数
	}


	if(true)
	{
		if (!bIsEmptyFile)
		{
			writeStream<<"\n";

		}

		QTime time = QTime::currentTime();
		writeStream<<QString(tr("Time:  %1:%2:%3")).arg(time.hour()).arg(time.minute()).arg(time.second())<<"\t";
		writeStream<<QString(tr("Product Number:  %1")).arg(nTempCheckedNum)<<"\t";
		writeStream<<QString(tr("Reject Number:  %1")).arg(nTempFailureNumFromIOcard)<<"\t";
		double dFailurRate = 1.0*nTempFailureNumFromIOcard/nTempCheckedNum*100;
		writeStream<<QString(tr("Reject Rate:  %1%")).arg(dFailurRate)<<"\n";

		writeStream<<tr("The statistical results")<<"\t";
		writeStream<<tr("Count")<<"\t";

		for (int i = 0;i<pMainFrm->m_sSystemInfo.iCamCount;i++)
		{
			if (i<9)
			{
				writeStream<<tr("Camera0%1").arg(i+1)<<"\t";
			}
			else
			{
				writeStream<<tr("Camera%1").arg(i+1)<<"\t";
			}
		}
		writeStream<<"\n";
		for (int j = 1;j<=pMainFrm->m_sErrorInfo.m_iErrorTypeCount;j++)
		{
			iIfShown[j] = 0;
			for (int i=0;i<pMainFrm->m_sSystemInfo.iCamCount;i++)
			{
				if (0 != nTempErrorTypeInfo[i].iErrorCountByType[j])
				{
					iIfShown[j] = 1;
				}

			}
			if (0 != iIfShown[j])
			{
				int k = pMainFrm->m_sErrorInfo.m_vstrErrorType.size();

				QString tempString = pMainFrm->m_sErrorInfo.m_vstrErrorType.at(j);
				writeStream<<tempString;
				if (tempString.length()<4)
				{
					//writeStream<<"\t";
					writeStream<<"\t"<<"\t";
				}
				else if (tempString.length()>=4&&tempString.length()<8)
				{
					writeStream<<"\t";
				}

				writeStream<<QString::number(nTempErrorTypeCount[j])<<"\t";

				for (int i = 0;i<pMainFrm->m_sSystemInfo.iCamCount;i++)
				{
					writeStream<<QString::number(nTempErrorTypeInfo[i].iErrorCountByType[j])<<"\t";
				}
				writeStream<<"\n";
			}
		}
		//writeStream<<tr("Count:")<<"\t";
		writeStream<<tr("Count:")<<"\t\t";
		int sum = 0;
		for (int i=0;i<pMainFrm->m_sSystemInfo.iCamCount;i++)
		{
			sum += nTempErrorCamCount[i];
		}

		writeStream<<QString::number(sum)<<"\t";

		for (int i = 0;i<pMainFrm->m_sSystemInfo.iCamCount;i++)
		{
			writeStream<<QString::number(nTempErrorTypeCount[i])<<"\t";
		}
		ret = true;

		writeStream<<"\n";

	}
	//
	if (!writeFile.open(QFile::Append | QIODevice::Text))
	{
		ret = false;
		pMainFrm->Logfile.write(tr("Write Count File Failure!"),OperationLog,0);
		return ret;
	}
	writeStream.flush();//写入流到文件

	writeFile.close();
	delete temp;
	pMainFrm->Logfile.write(tr("SaveCountInfoOver!"),OperationLog,0);
	//保存当前计数
	nLastCheckedNum = pMainFrm->m_sRunningInfo.m_checkedNum;
	nLastFailureNumFromIOcard = pMainFrm->m_sRunningInfo.m_failureNumFromIOcard;
	for (int i = 0; i < CAMERA_MAX_COUNT;i++)
	{
		nLastErrorCamCount[i] = m_iErrorCamCount[i];
		nLastErrorTypeInfo[i] = m_cErrorTypeInfo[i];
	}
	for (int i = 0; i < ERRORTYPE_MAX_COUNT;i++)
	{
		nLastErrorTypeCount[i]  = m_iErrorTypeCount[i];//每个错误类型的错误计数
	}


	return ret;
}
bool Widget_Count::slots_InsertCountHourInfo()
{
	QTime time = QTime::currentTime();
	int ihour = time.hour();
	// 	if ((0 == ihour) && (true == m_bIsErrorHour[23]))
	// 	{
	// 		for (int i = 0; i<24;i++)
	// 		{
	// 			m_bIsErrorHour[ihour] = false;
	// 		}
	// 		for (int i = 0; i < 24;i++)
	// 		{
	// 			for (int j = 0;j<ERRORTYPE_MAX_COUNT;j++)
	// 			{
	// 				m_iErrorHour[i][j] = 0;
	// 			}
	// 		}
	// 
	// 	}

	// 	if (m_bIsErrorHour[ihour])
	// 	{
	// 		return false;
	// 	}

	upDateListInfo();
	//更新按缺陷的报表
	while(m_modelHour->rowCount()>24 )
	{
		QList<QStandardItem*> listItem = m_modelHour->takeRow(24);
		qDeleteAll(listItem);
		listItem.clear();
	}

	for (int i = 0;i<ERRORTYPE_MAX_COUNT; i++)
	{
		m_timeByhour[ihour] = time;
		m_iErrorHour[ihour][i] = m_iErrorTypeCount[i];
		//		m_bIsErrorHour[ihour] = true;
	}

	QList<QStandardItem*> item;

	QStandardItem *item1 = new QStandardItem(QString::number(m_timeByhour[ihour].hour())+":"+QString::number(m_timeByhour[ihour].minute()));
	item<<item1;
	QStandardItem *item2 = new QStandardItem(QString::number(pMainFrm->m_sRunningInfo.m_checkedNum));
	item<<item2;
	QStandardItem *item3 = new QStandardItem(QString::number(pMainFrm->m_sRunningInfo.m_failureNumFromIOcard));
	item<<item3;

	for (int j = 1; j <= pMainFrm->m_sErrorInfo.m_iErrorTypeCount;j++)
	{
		QStandardItem *item4 = new QStandardItem(QString::number(m_iErrorHour[ihour][j]));
		item<<item4;
	}
	m_modelHour->insertRow(0,item);

	SaveCountHourInfo();
	return true;
}
bool Widget_Count::slots_InsertCountCameraInfo()
{
	QTime time = QTime::currentTime();
	int ihour = time.hour();
	int iBodyCount = 0;
	int iMouthCount = 0;
	int iButtomCount = 0;

	upDateListInfo();

	for (int i=0; i<pMainFrm->m_sSystemInfo.iCamCount; i++)
	{
		m_iHourErrorCamCount[i] = m_iErrorCamCount[i]-m_iLastErrorCamCount[i];
		m_iLastErrorCamCount[i] = m_iErrorCamCount[i];
	}
	//更新按缺陷的报表
	while(m_modelCamera->rowCount()>24 )
	{
		QList<QStandardItem*> listItem = m_modelCamera->takeRow(24);
		qDeleteAll(listItem);
		listItem.clear();
	}
	// 	for (int i = 0;i<ERRORTYPE_MAX_COUNT; i++)
	// 	{
	// 		m_timeByhour[ihour] = time;
	// 		m_iErrorHour[ihour][i] = m_iErrorTypeCount[i];
	////	m_bIsErrorHour[ihour] = true;
	// 	}

	QList<QStandardItem*> item;
	for (int i=0; i<pMainFrm->m_sSystemInfo.iCamCount; i++)
	{
		if (0 == pMainFrm->m_sCarvedCamInfo[i].m_iImageType)
		{
			iBodyCount += m_iHourErrorCamCount[i];
		}
		else if (1 == pMainFrm->m_sCarvedCamInfo[i].m_iImageType)
		{
			iMouthCount += m_iHourErrorCamCount[i];
		}
		else if (2 == pMainFrm->m_sCarvedCamInfo[i].m_iImageType)
		{
			iButtomCount += m_iHourErrorCamCount[i];
		}
	}
	QStandardItem *item1 = new QStandardItem(QString::number(m_timeByhour[ihour].hour())+":"+QString::number(m_timeByhour[ihour].minute()));
	item<<item1;
	QStandardItem *item2 = new QStandardItem(QString::number(iBodyCount));
	item<<item2;
	QStandardItem *item3 = new QStandardItem(QString::number(iMouthCount));
	item<<item3;
	QStandardItem *item4 = new QStandardItem(QString::number(iButtomCount));
	item<<item4;

	for (int j = 0; j < pMainFrm->m_sSystemInfo.iCamCount;j++)
	{
		QStandardItem *item5 = new QStandardItem(QString::number(m_iHourErrorCamCount[j]));
		item<<item5;
	}
	m_modelCamera->insertRow(0,item);

	SaveCountCameraInfo();
	return true;
}
bool Widget_Count::SaveCountHourInfo()
{
	pMainFrm->Logfile.write(tr("SaveCameraCountInfo!"),OperationLog,0);
	QDateTime dateTime = QDateTime::currentDateTime();

	QString strFileName = "CountInfo/HourCount/";

	strFileName = strFileName +	dateTime.toString("yy-MM-dd/hh.mm") + QString(".txt");

	QSettings setHourCount(strFileName,QSettings::IniFormat);
	setHourCount.setIniCodec("GBK");

	setHourCount.beginGroup("Count");
	setHourCount.setValue(QString("Product"),QString::number(pMainFrm->m_sRunningInfo.m_checkedNum));
	setHourCount.setValue(QString("Reject"),QString::number(pMainFrm->m_sRunningInfo.m_failureNumFromIOcard));
	int hour = dateTime.time().hour();
	for (int i = 1; i <= pMainFrm->m_sErrorInfo.m_iErrorTypeCount;i++)
	{
		setHourCount.setValue(QString("ErrorType%1").arg(i),QString::number(m_iErrorHour[hour][i]));
	}
	setHourCount.endGroup();
	return true;

}
bool Widget_Count::SaveCountCameraInfo()
{
	pMainFrm->Logfile.write(tr("SaveCameraCountInfo!"),OperationLog,0);
	QDateTime dateTime = QDateTime::currentDateTime();

	QString strFileName = "CountInfo/CameraCount/";

	// 	QDir *temp = new QDir;
	// 	bool exist = temp->exists(strFileName);
	// 	if(!exist)
	// 	{
	// 		bool ok = temp->mkdir(strFileName);
	// 	}
	// 
	// 	bool ret = false;
	// 	QDate date = QDate::currentDate();
	strFileName = strFileName +	dateTime.toString("yy-MM-dd/hh.mm") + QString(".txt");


	int iBodyCount = 0,iMouthCount = 0,iButtomCount = 0;

	for (int i=0; i<pMainFrm->m_sSystemInfo.iCamCount; i++)
	{
		if (0 == pMainFrm->m_sCarvedCamInfo[i].m_iImageType)
		{
			iBodyCount += m_iHourErrorCamCount[i];
		}
		else if (1 == pMainFrm->m_sCarvedCamInfo[i].m_iImageType)
		{
			iMouthCount += m_iHourErrorCamCount[i];
		}
		else if (2 == pMainFrm->m_sCarvedCamInfo[i].m_iImageType)
		{
			iButtomCount += m_iHourErrorCamCount[i];
		}
	}

	QSettings setCameraCount(strFileName,QSettings::IniFormat);
	setCameraCount.setIniCodec("GBK");

	setCameraCount.beginGroup("Count");
	setCameraCount.setValue(QString("Body"),QString::number(iBodyCount));
	setCameraCount.setValue(QString("Mouth"),QString::number(iMouthCount));
	setCameraCount.setValue(QString("Buttom"),QString::number(iButtomCount));
	for (int i = 0; i < pMainFrm->m_sSystemInfo.iCamCount;i++)
	{
		setCameraCount.setValue(QString("Camera%1").arg(i+1),QString::number(m_iHourErrorCamCount[i]));
	}
	setCameraCount.endGroup();


	return true;

}
void Widget_Count::InsertLastCountInfo()
{
	//插入上次小时计数
	pMainFrm->Logfile.write(tr("InsertLastCountInfo!"),OperationLog,0);
	QDateTime dateTime = QDateTime::currentDateTime();

	QString strFilePath = QString("CountInfo/HourCount/")+dateTime.toString("yy-MM-dd/");

	QDir dirHour;
	if (!dirHour.exists(strFilePath))
	{
		dirHour.mkpath(strFilePath);
		return;
	}
	dirHour.setPath(strFilePath);
	QFileInfoList fileListHour = dirHour.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot);
	if (fileListHour.isEmpty())
	{
		return;
	}
	QDateTime dateTimeLastHour = fileListHour.at(0).created();
	for (int i=0;i<fileListHour.size();++i)
	{
		QFileInfo fileInfoHour = fileListHour.at(i);
		//fileInfoHour.lastModified().toString("yyyy-MM-dd hh:mm:ss")
		QDateTime dateTimeModified = fileInfoHour.created();
		if (dateTimeLastHour<dateTimeModified)
		{
			dateTimeLastHour = dateTimeModified;
		}
	}

	int m_checkedNum,m_failureNumFromIOcard,m_iErrorHour[ERRORTYPE_MAX_COUNT+1];
	QString fileNameHour = strFilePath+dateTimeLastHour.toString("hh.mm") + QString(".txt");
	QSettings setHourCount(fileNameHour,QSettings::IniFormat);
	setHourCount.setIniCodec("GBK");

	setHourCount.beginGroup("Count");
	m_checkedNum = setHourCount.value("Product",0).toInt();
	m_failureNumFromIOcard = setHourCount.value("Reject",0).toInt();
	for (int i = 1; i <= pMainFrm->m_sErrorInfo.m_iErrorTypeCount;i++)
	{
		m_iErrorHour[i] = setHourCount.value(QString("ErrorType%1").arg(i),0).toInt();
	}
	setHourCount.endGroup();
	QList<QStandardItem*> itemhour;

	QStandardItem *itemHour1 = new QStandardItem(QString::number(dateTimeLastHour.time().hour())+":"+QString::number(dateTimeLastHour.time().minute()));
	itemhour<<itemHour1;
	QStandardItem *itemHour2 = new QStandardItem(QString::number(m_checkedNum));
	itemhour<<itemHour2;
	QStandardItem *itemHour3 = new QStandardItem(QString::number(m_failureNumFromIOcard));
	itemhour<<itemHour3;

	for (int j = 1; j <= pMainFrm->m_sErrorInfo.m_iErrorTypeCount;j++)
	{
		QStandardItem *itemHour4 = new QStandardItem(QString::number(m_iErrorHour[j]));
		itemhour<<itemHour4;
	}
	m_modelHour->insertRow(0,itemhour);

	//插入上次相机计数
	strFilePath = QString("CountInfo/CameraCount/")+dateTime.toString("yy-MM-dd/");

	QDir dirCamera;
	if (!dirCamera.exists(strFilePath))
	{
		return;
		dirCamera.mkpath(strFilePath);
	}
	dirCamera.setPath(strFilePath);
	QFileInfoList fileListCamera = dirCamera.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot);
	if (fileListCamera.isEmpty())
	{
		return;
	}
	QDateTime dateTimeLastCamera = fileListCamera.at(0).created();
	for (int i=0;i<fileListCamera.size();++i)
	{
		QFileInfo fileInfoHour = fileListCamera.at(i);
		//fileInfoHour.lastModified().toString("yyyy-MM-dd hh:mm:ss")
		QDateTime dateTimeModified = fileInfoHour.created();
		if (dateTimeLastCamera<dateTimeModified)
		{
			dateTimeLastCamera = dateTimeModified;
		}
	}

	int Body,Mouth,Buttom;//,m_iHourErrorCamCount[CAMERA_MAX_COUNT];
	QString fileNameCamera = strFilePath+dateTimeLastHour.toString("hh.mm") + QString(".txt");

	QSettings setCameraCount(fileNameCamera,QSettings::IniFormat);
	setCameraCount.setIniCodec("GBK");

	setCameraCount.beginGroup("Count");
	Body = setCameraCount.value("Body",0).toInt();
	Mouth = setCameraCount.value("Mouth",0).toInt();
	Buttom = setHourCount.value("Buttom",0).toInt();
	//setCameraCount.setValue(QString("Body"),QString::number(iBodyCount));
	//setCameraCount.setValue(QString("Mouth"),QString::number(iMouthCount));
	//setCameraCount.setValue(QString("Buttom"),QString::number(iButtomCount));
	for (int i = 0; i < pMainFrm->m_sSystemInfo.iCamCount;i++)
	{
		m_iHourErrorCamCount[i] = setCameraCount.value(QString("Camera%1").arg(i+1),0).toInt();
		//setCameraCount.setValue(QString("Camera%1").arg(i+1),QString::number(m_iHourErrorCamCount[i]));
	}


	QList<QStandardItem*> itemCamera;
	QStandardItem *itemCamera1 = new QStandardItem(QString::number(dateTimeLastCamera.time().hour())+":"+QString::number(dateTimeLastCamera.time().minute()));
	itemCamera<<itemCamera1;
	QStandardItem *itemCamera2 = new QStandardItem(QString::number(Body));
	itemCamera<<itemCamera2;
	QStandardItem *itemCamera3 = new QStandardItem(QString::number(Mouth));
	itemCamera<<itemCamera3;
	QStandardItem *itemCamera4 = new QStandardItem(QString::number(Buttom));
	itemCamera<<itemCamera4;

	for (int j = 0; j < pMainFrm->m_sSystemInfo.iCamCount;j++)
	{
		QStandardItem *itemCamera5 = new QStandardItem(QString::number(m_iHourErrorCamCount[j]));
		itemCamera<<itemCamera5;
	}
	m_modelCamera->insertRow(0,itemCamera);

}

void Widget_Count::writeCountInfo(QFile* fileCount)
{
	QTextStream createStream(fileCount);
	QTime time = QTime::currentTime();
	createStream<<QString(tr("Time: %1:%2:%3")).arg(time.hour()).arg(time.minute()).arg(time.second())<<"\n";
	createStream<<tr("The statistical results\t");
	for (int i = 0;i<pMainFrm->m_sSystemInfo.iCamCount;i++)
	{
		createStream<<tr("Camera%1\t").arg(i+1);
	}
	createStream<<tr("Count\n");
	for (int j = 0;j<=pMainFrm->m_sErrorInfo.m_iErrorTypeCount;j++)
	{
		for (int i=0;i<pMainFrm->m_sSystemInfo.iCamCount;i++)
		{
			if (0 != m_cErrorTypeInfo[i].iErrorCountByType[j])
			{
				iIfShown[j] = 1;
			}
		}

		if (0 != iIfShown[j])
		{
			QString tempString = pMainFrm->m_sErrorInfo.m_vstrErrorType.at(j);
			createStream<<tempString;
			if (tempString.length()<4)
			{
				createStream<<"\t";
			}
			else if (tempString.length()>=4&&tempString.length()<8)
			{
				createStream<<"\t";
			}
			for (int i = 0;i<pMainFrm->m_sSystemInfo.iCamCount;i++)
			{
				createStream<<QString::number(m_cErrorTypeInfo[i].iErrorCountByType[j])<<"\t";
			}
			createStream<<QString::number(m_iErrorTypeCount[j]);
			createStream<<"\n";
		}
	}
	createStream<<tr("Count\t");
	for (int i = 0;i<pMainFrm->m_sSystemInfo.iCamCount;i++)
	{
		createStream<<QString::number(m_iErrorCamCount[i])<<"\t";
	}
	createStream.flush();

	pMainFrm->Logfile.write(tr("Save statistics"),OperationLog);

}
void Widget_Count::slots_timerClearCountInfo()
{
	// 	Qt::HANDLE ThreadId = QThread::currentThreadId(); 

	QTime time = QTime::currentTime();
	int iTime = time.hour()*10000+time.minute()*100+time.second();

	int iShift[3];
	iShift[0] = shift1.hour()*10000 + shift1.minute()*100 + shift1.second();
	iShift[1] = shift2.hour()*10000 + shift2.minute()*100 + shift2.second();
	iShift[2] = shift3.hour()*10000 + shift3.minute()*100 + shift3.second();

	for (int i = 0; i<2; i++)
	{
		if (iShift[i] > iShift[i+1])
		{
			int temp = iShift[i];
			iShift[i] = iShift[i+1];
			iShift[i+1] = temp;
		}
	}
	if (iTime>=iShift[0] && iTime<iShift[1])
	{
		if (!isClear1)
		{
			slots_SaveALLCountInfo();
			pMainFrm->MonitorLicense();
			bIsAutoSetZero = true;
			ClearAllData();
			isClear1 = true;
			isClear2 = false;
			isClear3 = false;
		}
	}
	if (iTime>=iShift[1] && iTime<iShift[2])
	{
		if (!isClear2)
		{
			slots_SaveALLCountInfo();
			pMainFrm->MonitorLicense();
			bIsAutoSetZero = true;
			ClearAllData();
			isClear1 = false;
			isClear2 = true;
			isClear3 = false;

		}
	}	
	if (iTime<iShift[0] || iTime>=iShift[2])
	{
		if (!isClear3)
		{
			slots_SaveALLCountInfo();
			pMainFrm->MonitorLicense();
			bIsAutoSetZero = true;
			ClearAllData();
			isClear1 = false;
			isClear2 = false;
			isClear3 = true;

		}
	}

}
void Widget_Count::slots_saveRecordOK()
{
	pMainFrm->m_sSystemInfo.bSaveRecord = widgetCountSet->ui.checkBox_saveRecord->isChecked();
	if (pMainFrm->m_sSystemInfo.bSaveRecord)
	{
		if (widgetCountSet->ui.radioButton_30minite->isChecked())
		{
			pMainFrm->m_sSystemInfo.iSaveRecordInterval = 30;
		}
		else if (widgetCountSet->ui.radioButton_60minite->isChecked())
		{
			pMainFrm->m_sSystemInfo.iSaveRecordInterval = 60;
		}

		if (!timerSaveHourList->isActive())
		{
			//timerSaveHourList->setInterval(60000);//每分钟触发一次  
			timerSaveHourList->start(60000);  	//启动定时器    
		}
	}
	else
	{  
		timerSaveHourList->stop();  	//关闭定时器    
	}
	bSaveRecord = pMainFrm->m_sSystemInfo.bSaveRecord;
	iSaveRecordInterval = pMainFrm->m_sSystemInfo.iSaveRecordInterval;

	QSettings saveRecordSet(pMainFrm->m_sConfigInfo.m_strConfigPath,QSettings::IniFormat);
	saveRecordSet.setIniCodec(QTextCodec::codecForName("GBK"));
	saveRecordSet.beginGroup("system");
	saveRecordSet.setValue(QString("bSaveRecord"),bSaveRecord);
	saveRecordSet.setValue(QString("iSaveRecordInterval"),iSaveRecordInterval);
	saveRecordSet.endGroup();

}
void Widget_Count::slots_saveRecordCancel()
{
	widgetCountSet->ui.checkBox_saveRecord->setChecked(bSaveRecord);

	if (30 == iSaveRecordInterval)
	{
		widgetCountSet->ui.radioButton_30minite->setChecked(true);
	}
	else if (60 == iSaveRecordInterval)
	{
		widgetCountSet->ui.radioButton_60minite->setChecked(true);
	}

}
void Widget_Count::slots_shiftOK()
{
	pMainFrm->m_sSystemInfo.bAutoSetZero = widgetCountSet->ui.checkBox_AutoSetZero->isChecked();

	pMainFrm->m_sSystemInfo.shift1 = widgetCountSet->ui.timeEdit_shift1->time();
	pMainFrm->m_sSystemInfo.shift2 = widgetCountSet->ui.timeEdit_shift2->time();
	pMainFrm->m_sSystemInfo.shift3 = widgetCountSet->ui.timeEdit_shift3->time();
	
	bAutoSetZero = pMainFrm->m_sSystemInfo.bAutoSetZero;
	shift1 = pMainFrm->m_sSystemInfo.shift1;
	shift2 = pMainFrm->m_sSystemInfo.shift2;
	shift3 = pMainFrm->m_sSystemInfo.shift3;
	if (bAutoSetZero)
	{
		connect(timerSaveHourList, SIGNAL(timeout()), this, SLOT(slots_timerClearCountInfo()));   
	}
	else
	{
		disconnect(timerSaveHourList, SIGNAL(timeout()), this, SLOT(slots_timerClearCountInfo()));   
	}

	QSettings shiftSet(pMainFrm->m_sConfigInfo.m_strConfigPath,QSettings::IniFormat);
	shiftSet.setIniCodec(QTextCodec::codecForName("GBK"));
	shiftSet.beginGroup("system");
	shiftSet.setValue(QString("bAutoSetZero"),bAutoSetZero);
	shiftSet.setValue(QString("shift1"),shift1.hour()*10000+shift1.minute()*100+shift1.second());
	shiftSet.setValue(QString("shift2"),shift2.hour()*10000+shift2.minute()*100+shift2.second());
	shiftSet.setValue(QString("shift3"),shift3.hour()*10000+shift3.minute()*100+shift3.second());
	shiftSet.endGroup();

	QMessageBox::information(this,tr("Shift"),tr("Shift Settting Succeed!"),QMessageBox::Ok);

}
void Widget_Count::slots_shiftCancel()
{
	widgetCountSet->ui.checkBox_AutoSetZero->setChecked(bAutoSetZero);
	widgetCountSet->ui.timeEdit_shift1->setTime(shift1);
	widgetCountSet->ui.timeEdit_shift2->setTime(shift2);
	widgetCountSet->ui.timeEdit_shift3->setTime(shift3);

}
void Widget_Count::slots_turn()
{
	if (0 == pStackedCount->currentIndex())
	{
		//widgetCountSet->setVisible(false);
		pStackedCount->setCurrentIndex(1);
	}
	else
	{
		//widgetCountSet->setVisible(true);
		pStackedCount->setCurrentIndex(0);
	}
}
void Widget_Count::slots_updateInfo()
{
	double total = pMainFrm->m_sRunningInfo.m_checkedNum;
	double failur = pMainFrm->m_sRunningInfo.m_failureNumFromIOcard;
	double Readmodle = pMainFrm->m_sRunningInfo.nModelCheckedCount;
	double failurRate,ModleRate;;
	if (0 == total)
	{
		failurRate = 0;
		ModleRate = 0;
	}
	else
	{
		failurRate = (failur/total)*100;
		ModleRate = (Readmodle/total)*100;
	}
	widgetCountSet->ui.label_total->setText(tr("Product Number")+"\n"+QString::number(total));
	widgetCountSet->ui.label_failur->setText(tr("Reject Number")+"\n"+QString::number(failur));
	widgetCountSet->ui.label_failurRate->setText(tr("Reject Rate")+"\n"+QString::number(failurRate,'f',2)+"%");
	//widgetCountSet->ui.label_modleRate->setText(QString::fromLocal8Bit("识别率:")+"\n"+QString::number(ModleRate,'f',2)+"%");
}

void Widget_Count::ClearAllData(int resultFromQ)
{
	bIsAutoSetZero = false;
	pMainFrm->MonitorLicense();
	pMainFrm->Logfile.write(tr("Set zero!"),OperationLog);
	pMainFrm->m_sRunningInfo.m_mutexRunningInfo.lock();
	//手动清零，保存历史数据,后续插入数据库
	if(resultFromQ)
	{
		m_TemporaryData.m_AllCount += pMainFrm->m_sRunningInfo.m_checkedNum - m_LastData.m_AllCount;
		m_TemporaryData.m_FailCount += pMainFrm->m_sRunningInfo.m_failureNumFromIOcard - m_LastData.m_FailCount;
		for (int i=1;i<ERRORTYPE_MAX_COUNT;i++)
		{
			m_TemporaryData.m_CameraTypeCount[i] += pMainFrm->m_sRunningInfo.m_iErrorTypeCount[i] - m_LastData.m_CameraTypeCount[i];
		}
		for (int i=1;i<CAMERA_MAX_COUNT;i++)
		{
			for(int j=1;j<50;j++)
			{
				m_TemporaryData.m_TypeCount[i][j] += pMainFrm->m_sRunningInfo.m_cErrorTypeInfo[i-1].iErrorCountByType[j] - m_LastData.m_TypeCount[i][j];
			}
		}
	}else{
		//变量清零、更新表格、
 		m_WidgetReport->clearReport();
		m_TemporaryData.TemporaryDataClear();
	}
	//清零整点数据
	m_LastData.TemporaryDataClear();
	QSettings LoadLastData(pMainFrm->SaveDataPath,QSettings::IniFormat);
	LoadLastData.setIniCodec(QTextCodec::codecForName("GBK"));
	QString strSession;
	QDateTime dateTime=QDateTime::currentDateTime();
	strSession = QString("LastTimeDate/LastTime");
	LoadLastData.setValue(strSession,dateTime.toString("yyyy-MM-dd hh:mm"));
	strSession = QString("LastTimeDate/Checknumber");
	LoadLastData.setValue(strSession,0);
	strSession = QString("LastTimeDate/Failurenumber");
	LoadLastData.setValue(strSession,0);
	for (int i=1;i<ERRORTYPE_MAX_COUNT;i++)
	{
		strSession = QString("LastTimeDate/ErrorType_%1_count").arg(i);
		LoadLastData.setValue(strSession,0);
	}
	for (int i=1;i<CAMERA_MAX_COUNT;i++)
	{
		for(int j=1;j<ERRORTYPE_MAX_COUNT;j++)
		{
			strSession = QString("LastTimeDate/Camera%1_ErrorType%2").arg(i).arg(j);
			LoadLastData.setValue(strSession,0);
		}
	}

	pMainFrm->m_sRunningInfo.m_checkedNum = 0;
	//pMainFrm->m_sRunningInfo.m_passNum = 0;
	pMainFrm->m_sRunningInfo.m_failureNum = 0;
	//pMainFrm->m_sRunningInfo.m_passNum2 = 0;
	pMainFrm->m_sRunningInfo.m_failureNum2 = 0;

	pMainFrm->m_sRunningInfo.m_failureNumFromIOcard = 0;
	pMainFrm->m_sRunningInfo.nModelCheckedCount = 0;

	//新增变量清零by zl
	for(int i=0;i<4;i++)
	{
		pMainFrm->m_sRunningInfo.nGSoap_ErrorCamCount[i] = 0 ;
		pMainFrm->m_sRunningInfo.nGSoap_ErrorTypeCount[i] = 0;
	}

	for (int i=0;i<CAMERA_MAX_COUNT;i++)
	{
		pMainFrm->m_sRunningInfo.m_iErrorCamCount[i] = 0;
		pMainFrm->m_sRunningInfo.m_cErrorTypeInfo[i].Clear();
	}
	for (int i=0;i<ERRORTYPE_MAX_COUNT;i++)
	{
		pMainFrm->m_sRunningInfo.m_iErrorTypeCount[i] = 0;
	}

	pMainFrm->m_sRunningInfo.m_mutexRunningInfo.unlock();

	nLastCheckedNum = 0;
	nLastFailureNumFromIOcard = 0;
	for (int i = 0; i < CAMERA_MAX_COUNT;i++)
	{
		nLastErrorCamCount[i] = 0;
		nLastErrorCamRate[i] = 0;
		nLastErrorTypeInfo[i].Clear();;
	}
	for (int i = 0; i < ERRORTYPE_MAX_COUNT;i++)
	{
		nLastErrorTypeCount[i] = 0;//每个错误类型的错误计数
	}


	timerSaveHourList->stop();
	timerSaveHourList->start();
	int aa= m_modelCount->rowCount();
	while(m_modelCount->rowCount()>0 )
	{
		QList<QStandardItem*> listItem = m_modelCount->takeRow(0);
		qDeleteAll(listItem);
		listItem.clear();
	}
	while(m_modelHour->rowCount()>0 )
	{
		QList<QStandardItem*> listItem = m_modelHour->takeRow(0);
		qDeleteAll(listItem);
		listItem.clear();
	}	
	if (pMainFrm->m_sSystemInfo.iIsCameraCount)
	{
		while(m_modelCamera->rowCount()>0 )
		{
			QList<QStandardItem*> listItem = m_modelCamera->takeRow(0);
			qDeleteAll(listItem);
			listItem.clear();
		}
	}
	for (int i=0;i < 24;i++)
	{
		for (int j=0;j<ERRORTYPE_MAX_COUNT;j++)
		{
			m_iErrorHour[i][j] = 0;
		}
	}
	for (int i=0; i<pMainFrm->m_sSystemInfo.iCamCount; i++)
	{
		m_iLastErrorCamCount[i] = 0;
	}
	//删除保存的临时数据
	QFile fileTemp(pMainFrm->SaveDataPath);
	fileTemp.remove();
}
bool Widget_Count::slots_SaveALLCountInfo()
{
	upDateListInfo();
	bool bIsEmptyFile = false;
	QString strFileName = pMainFrm->m_sConfigInfo.m_strAppPath + "CountInfo/AllCount/";
	QDir *temp = new QDir;
	bool exist = temp->exists(strFileName);
	if(!exist)
	{
		bool ok = temp->mkdir(strFileName);
	}
	bool ret = false;
	QDate date = QDate::currentDate();
	strFileName = strFileName +	date.toString(Qt::ISODate) + ".txt";

	if(!QFile::exists(strFileName))
	{
		QFile createFile(strFileName);
		if(!createFile.open(QFile::WriteOnly | QIODevice::Text))
		{
			pMainFrm->Logfile.write(tr("Create Count File Failure!"),OperationLog,0);
			return ret;
		}

		bIsEmptyFile = true;
		createFile.close();
	}

	QFile readFile(strFileName);

	if (!readFile.open(QFile::Append | QIODevice::Text))
	{
		ret = false;
		pMainFrm->Logfile.write(tr("Read Count File Failure!"),OperationLog,0);
		return ret;
	}

	QFile writeFile(strFileName);
	QTextStream writeStream(&writeFile);

	if(true)
	{
		if (!bIsEmptyFile)
		{
			writeStream<<"\n";

		}

		QTime time = QTime::currentTime();
		writeStream<<QString(tr("Time:  %1:%2:%3")).arg(time.hour()).arg(time.minute()).arg(time.second())<<"\t";
		writeStream<<QString(tr("Product Number:  %1")).arg(pMainFrm->m_sRunningInfo.m_checkedNum)<<"\t";
		writeStream<<QString(tr("Reject Number:  %1")).arg(pMainFrm->m_sRunningInfo.m_failureNumFromIOcard)<<"\t";
		double dFailurRate = 1.0*pMainFrm->m_sRunningInfo.m_failureNumFromIOcard/pMainFrm->m_sRunningInfo.m_checkedNum*100;
		writeStream<<QString(tr("Reject Rate:  %1%")).arg(dFailurRate)<<"\n";

		int iTempbody = 0, iTempfinish = 0, iTempBottom=0;
		for(int i=0;i<=pMainFrm->m_sSystemInfo.iCamCount;i++)
		{
			if(0 == pMainFrm->m_sCarvedCamInfo[i].m_iImageType)
			{
				iTempbody += pMainFrm->m_sRunningInfo.m_iErrorCamCount[i];
			}
			else if(1 == pMainFrm->m_sCarvedCamInfo[i].m_iImageType)
			{
				iTempfinish += pMainFrm->m_sRunningInfo.m_iErrorCamCount[i];
			}		
			else if(2 == pMainFrm->m_sCarvedCamInfo[i].m_iImageType)
			{
				iTempBottom += pMainFrm->m_sRunningInfo.m_iErrorCamCount[i];
			}			
		}
		writeStream<<QString(tr("Reject Number of body:  %1")).arg(iTempbody)<<"\t";
		writeStream<<QString(tr("Reject Number of finish:  %1")).arg(iTempfinish)<<"\t";
		writeStream<<QString(tr("Reject Number of bottom:  %1")).arg(iTempBottom)<<"\n";
		for(int i=1;i<=pMainFrm->m_sErrorInfo.m_iErrorTypeCount;i++)
		{
			writeStream<<pMainFrm->m_sErrorInfo.m_vstrErrorType.at(i) + ":" + QString::number(pMainFrm->m_sRunningInfo.m_iErrorTypeCount[i])<<"\t";

		}
		writeStream<<"\n";
		writeStream<<tr("The statistical results")<<"\t";
		writeStream<<tr("Count")<<"\t";
		writeStream<<QString::fromLocal8Bit("错误占比")<<"\t";
		for (int i = 0;i<pMainFrm->m_sSystemInfo.iCamCount;i++)
		{
			if (i<9)
			{
				writeStream<<tr("Camera0%1").arg(i+1)<<"\t";
			}
			else
			{
				writeStream<<tr("Camera%1").arg(i+1)<<"\t";
			}
		}
		writeStream<<"\n";
		QMap<int,int> tempList;
		for (int j = 1;j<=pMainFrm->m_sErrorInfo.m_iErrorTypeCount;j++)
		{
			for (int i=0;i<pMainFrm->m_sSystemInfo.iCamCount;i++)
			{
				if (0 != m_cErrorTypeInfo[i].iErrorCountByType[j])
				{
					iIfShown[j] = 1;
					tempList.insert(m_iErrorTypeCount[j],j);
				}
			}
		}
		int sum = 0;
		for (int i=0;i<pMainFrm->m_sSystemInfo.iCamCount;i++)
		{
			sum += m_iErrorCamCount[i];
		}
		QMap<int,int>::Iterator its;
		for(its = tempList.begin();its!= tempList.end();its++)
		{
			QString tempString = pMainFrm->m_sErrorInfo.m_vstrErrorType.at(its.value());
			writeStream<<tempString;
			if (tempString.length()<4)
			{
				writeStream<<"\t"<<"\t";
			}
			else if (tempString.length()>=4&&tempString.length()<8)
			{
				writeStream<<"\t";
			}

			writeStream<<QString::number(m_iErrorTypeCount[its.value()])<<"\t";
			float tempNumber=(float)m_iErrorTypeCount[its.value()]/sum*100;

			writeStream<<QString::number(tempNumber,'f',2)+QString("%")<<"\t"<<"\t";

			for (int i = 0;i<pMainFrm->m_sSystemInfo.iCamCount;i++)
			{
				writeStream<<QString::number(m_cErrorTypeInfo[i].iErrorCountByType[its.value()])<<"\t";
			}
			writeStream<<"\n";
		}
		writeStream<<tr("Count:")<<"\t\t";

		writeStream<<QString::number(sum)<<"\t"<<"\t"<<"\t";

		for (int i = 0;i<pMainFrm->m_sSystemInfo.iCamCount;i++)
		{
			writeStream<<QString::number(m_iErrorCamCount[i])<<"\t";
		}
		ret = true;

		writeStream<<"\n";

	}
	//
	if (!writeFile.open(QFile::Append | QIODevice::Text))
	{
		ret = false;
		pMainFrm->Logfile.write(tr("Write Count File Failure!"),OperationLog,0);
		return ret;
	}
	writeStream.flush();//写入流到文件

	writeFile.close();
	delete temp;
	pMainFrm->Logfile.write(tr("SaveCountInfoOver!"),OperationLog,0);
	if (pMainFrm->m_sSystemInfo.iIsSaveCountInfoByTime)
	{
		SaveCountInfoByTime();
	}

	return ret;
}