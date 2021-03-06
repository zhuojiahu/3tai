#include <QDir>
#include <QStringList>
#include <QTextCodec>
#include <QInputDialog>
#include <QFileDialog>
#include <QPushButton>

//#include <QFileDialog>
#include "widget_Management.h"

#include "glasswaredetectsystem.h"

extern GlasswareDetectSystem *pMainFrm;

MyCameraPoButton::MyCameraPoButton(QWidget *parent)
	: QPushButton(parent)

{
}
MyCameraPoButton::~MyCameraPoButton()
{

}

WidgetManagement::WidgetManagement(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.tableCate->setFont(QFont("微软雅黑",16,3));
	ui.tableCate->setColumnCount(3);
	QStringList strHoriHeader;
	strHoriHeader<<tr("Image")<<tr("Category Info")<<tr("Camera Position");
	ui.tableCate->setHorizontalHeaderLabels(strHoriHeader);
	ui.tableCate->setEditTriggers(QAbstractItemView::NoEditTriggers);//不可编辑
	ui.tableCate->setSelectionBehavior(QAbstractItemView::SelectRows);//整行选择
	ui.tableCate->setSelectionMode(QAbstractItemView::SingleSelection);//单目标选择
	ui.tableCate->verticalHeader()->setVisible(false);
//	ui.tableCate->setColumnWidth(3,150);
	UpdateTable();
	connect(ui.tableCate,SIGNAL(cellClicked(int,int)),this,SLOT(slots_cellClicked(int,int)));
	connect(ui.tableCate,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(slots_load()));
	connect(ui.btnNewCate,SIGNAL(clicked()),this,SLOT(slots_new()));
	connect(ui.btnLoadCate,SIGNAL(clicked()),this,SLOT(slots_load()));
	connect(ui.btnBackupCate,SIGNAL(clicked()),this,SLOT(slots_backup()));
	connect(ui.btnDeleteCate,SIGNAL(clicked()),this,SLOT(slots_delete()));
	connect(ui.btnExport,SIGNAL(clicked()),this,SLOT(slots_export()));
	connect(ui.btnImport,SIGNAL(clicked()),this,SLOT(slots_import()));

	connect(this, SIGNAL(signals_clearTable()), pMainFrm->widget_carveSetting->errorList_widget, SLOT(slots_clearTable()));

}
WidgetManagement::~WidgetManagement()
{
	delete cellWidget;
}
void WidgetManagement::slots_intoWidget()
{
	iSelectRow = iCurModelRow;
	ui.widget_Managment->setVisible(true);
}
bool WidgetManagement::leaveWidget()
{
	return true;
}

void WidgetManagement::UpdateTable()
{
	Sleep(200);//等待文本操作完成
	ui.tableCate->clearContents();//先清空
	ui.tableCate->setRowCount(0);

	QString strModelName = pMainFrm->m_sSystemInfo.m_strModelName;//.section('\\',-1);//当前产品名称
	QDir dir(pMainFrm->m_sConfigInfo.m_strAppPath+"ModelInfo/");
	dirList = dir.entryInfoList(QDir::AllDirs | QDir::Hidden | QDir::NoDotAndDotDot);

	for (int i=0;i<dirList.size();++i)
	{
		int iRowNo = ui.tableCate->rowCount();
		ui.tableCate->insertRow(iRowNo);
		QFileInfo dirInfo = dirList.at(i);
		QString t123 = dirInfo.absoluteFilePath();
		QFileInfo modelInfo(dirInfo.absoluteFilePath()+"/ModelPara.ini");
		QFileInfo imageInfo(dirInfo.absoluteFilePath()+"/modle.bmp");

		cellWidget = new WidgetMyCellWidget(this);
		QWidget *widgetButton = new QWidget(this);

		MyCameraPoButton *buttonCameraPo = new MyCameraPoButton(widgetButton);
 		buttonCameraPo->iNumber = i;
		buttonCameraPo->setFixedSize(100,60);
		buttonCameraPo->setText(tr("Position"));

		QFont font1;
		font1.setPointSize(12);
		buttonCameraPo->setFont(font1);

		connect(cellWidget->action_selectImage,SIGNAL(triggered()),this,SLOT(slots_action_selectImage()));
		if (imageInfo.exists())
		{
			QPixmap pixmap;

			pixmap.load(dirInfo.absoluteFilePath()+"/modle.bmp");
			cellWidget->addImg(pixmap);
		}
		else
		{
			cellWidget->addText(dirInfo.completeBaseName());
		}
		ui.tableCate->setCellWidget(i,0,cellWidget);
		ui.tableCate->setCellWidget(i,1,widgetButton);
		ui.tableCate->setItem(i,2,new QTableWidgetItem(tr("Name:")+dirInfo.completeBaseName()+"\n"+tr("Last modified:")+dirInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss")));
		QString str1 = dirInfo.completeBaseName();
		if (strModelName == dirInfo.completeBaseName())
		{
			iCurModelRow = i;
			ui.tableCate->item(i,2)->setTextColor(QColor(255,0,0));
		}
	}
	ui.tableCate->setColumnWidth(0,100);
	ui.tableCate->verticalHeader()->setDefaultSectionSize(80); //设置行高
	//ui.tableCate->resizeColumnsToContents();//行列大小与内容匹配
	//ui.tableCate->resizeRowsToContents();
	ui.tableCate->horizontalHeader()->setStretchLastSection(true);//行自动填充宽度
	iSelectRow = -1;

}

void WidgetManagement::slots_cellClicked(int row,int col)
{
	iSelectRow = row;
}

void WidgetManagement::slots_new()
{
	bool ok;
	QString strModelPath;
//	QString	strSrcPath;
	QDir dir;
// 	strSrcPath = ui.leditCateName->text();
	QString strModelName = QInputDialog::getText(this, tr("Input a new name"),tr("New name:"), QLineEdit::Normal,"", &ok);
	if (!ok)
	{
		return;
	}
	
	strModelPath = pMainFrm->m_sConfigInfo.m_strAppPath+"ModelInfo/"+strModelName;//+ui.leditCateName->text();
	if ("" == strModelName)
	{
		QMessageBox::information(this,tr("Information"),tr("Please input model name."));
	} 
	else if (dir.exists(strModelPath))
	{
		QMessageBox::information(this,tr("Information"),tr("Model name already exist."));
		return;
	}
	dir.mkpath(strModelPath);

	QString strSrcPath = pMainFrm->m_sConfigInfo.m_strAppPath + "ModelInfo/" + pMainFrm->m_sSystemInfo.m_strModelName;
	BackupCate(strModelPath,strSrcPath);

	UpdateTable();
// 	ui.tableCate->setCurrentCell(0,0);
	iSelectRow = -1;
	pMainFrm->Logfile.write((tr("New model:[%1]").arg(strModelName.section("/",-1))),OperationLog);
}
void WidgetManagement::SeverAdd(QString strModelName)
{
	QString strModelPath;
	QDir dir;
	strModelPath = pMainFrm->m_sConfigInfo.m_strAppPath+"ModelInfo/"+strModelName;
	dir.mkpath(strModelPath);
	QString strSrcPath = pMainFrm->m_sConfigInfo.m_strAppPath + "ModelInfo/" + pMainFrm->m_sSystemInfo.m_strModelName;
	BackupCate(strModelPath,strSrcPath);
	UpdateTable();
	iSelectRow = -1;
}
void WidgetManagement::SeverSelect(QString strModelName)
{
	pMainFrm->m_sSystemInfo.m_strModelName = strModelName;
	pMainFrm->m_sConfigInfo.m_strGrabInfoPath = pMainFrm->m_sConfigInfo.m_strAppPath + "ModelInfo/" + strModelName + "/GrabInfo.ini";

	pMainFrm->InitImage();
	pMainFrm->InitCheckSet();

	pMainFrm->m_sSystemInfo.LastModelName = strModelName;
	//界面
	UpdateTable();
	iSelectRow = -1;
	SaveModelNeme(strModelName);
	emit signals_clearTable();
}
void WidgetManagement::SeverDelete(QString nDeleteModeName)
{
	int nSelectRow = 0;
	for(int i=0;i<dirList.count();i++)
	{
		if(dirList.at(i).absoluteFilePath().section("/",-1) == nDeleteModeName)
		{
			nSelectRow = i;
			break;
		}
	}
	QString strDirPath = dirList.at(nSelectRow).absoluteFilePath();
	DeleteCate(strDirPath);
	UpdateTable();
	iSelectRow = -1;
}
void WidgetManagement::slots_load(bool bCurModel)
{
	if (pMainFrm->m_sRunningInfo.m_bCheck)
	{
		QMessageBox::information(this,tr("Information"),tr("Please stop detection!"));
		return;
	}
	if (iSelectRow == iCurModelRow && !bCurModel)
	{
		QMessageBox::information(this,tr("Information"),tr("Current product model is already loaded!"));
		return;
	}
	if ((iSelectRow > dirList.size()-1)||iSelectRow < 0)
	{
		QMessageBox::information(this,tr("Information"),tr("Product model not selected!"));
		return;
	}
	QString strDirPath = dirList.at(iSelectRow).absoluteFilePath();

	if (QMessageBox::No == QMessageBox::question(this,tr("Load"),
		tr("Are you sure to load:<font color=red>[%1]</font>?").arg(strDirPath.section("/",-1)),
		QMessageBox::Yes | QMessageBox::No))
	{
		return;
	}
	for (int i = 0; i < pMainFrm->m_sSystemInfo.iRealCamCount;i++)
	{
		pMainFrm->m_sRealCamInfo[i].m_bGrabIsStart = FALSE;
	}
	pMainFrm->m_sSystemInfo.m_strModelName = strDirPath.section("/",-1);
	pMainFrm->m_sConfigInfo.m_strGrabInfoPath = pMainFrm->m_sConfigInfo.m_strAppPath + "ModelInfo/" + pMainFrm->m_sSystemInfo.m_strModelName + "/GrabInfo.ini";
	
	QSettings iniCameraSet(pMainFrm->m_sConfigInfo.m_strGrabInfoPath,QSettings::IniFormat);
	QString strShuter,strTrigger;
	for(int i = 0; i < pMainFrm->m_sSystemInfo.iRealCamCount; i++)
	{
		strShuter = QString("/Shuter/Grab_%1").arg(i);
		pMainFrm->m_sRealCamInfo[i].m_iShuter=iniCameraSet.value(strShuter,20).toInt();
	}
	
	//pMainFrm->ReadCorveConfig();
	pMainFrm->InitImage();
	pMainFrm->InitCheckSet();

	pMainFrm->m_sSystemInfo.LastModelName = (QString)(strDirPath.section("/",-1));
	//界面
	UpdateTable();
	iSelectRow = -1;
	pMainFrm->Logfile.write(QString(tr("Loading the template:")+strDirPath.section("/",-1)),OperationLog);

	SaveModelNeme(strDirPath);

	emit signals_clearTable();

	for (int i = 0; i < pMainFrm->m_sSystemInfo.iRealCamCount;i++)
	{
		pMainFrm->m_sRealCamInfo[i].m_bGrabIsStart = TRUE;
	}
}
void WidgetManagement::SaveModelNeme(QString strDirPath)
{
	//写配置文件
	QSettings modelInfoSet(pMainFrm->m_sConfigInfo.m_strConfigPath,QSettings::IniFormat);
	modelInfoSet.setIniCodec(QTextCodec::codecForName("GBK"));
	//modelInfoSet.beginGroup("system");
	//modelInfoSet.setValue(QString("LastModelName"),(QString)(strDirPath.section("/",-1)));
	//modelInfoSet.endGroup();
	QString str = strDirPath.section("/",-1);
	modelInfoSet.setValue("/system/LastModelName",str);	//读取上次使用模板
}
void WidgetManagement::slots_backup()
{
 	bool ok;
 	if ((iSelectRow > dirList.size()-1)||iSelectRow < 0)
 	{
 		QMessageBox::information(this,tr("Information"),tr("Product model not selected!"));
 		return;
 	}

 	QString strName = QInputDialog::getText(this, 
											tr("Input a new name"),
											tr("New name:"), 
											QLineEdit::Normal,
											dirList.at(iSelectRow).baseName() + " - Backup", 
											&ok);

 	if (!ok || strName.isEmpty())
 		return;

 	QString strSrcPath = dirList.at(iSelectRow).absoluteFilePath();
 	QString strDestPath = pMainFrm->m_sConfigInfo.m_strAppPath+"ModelInfo/"+strName;
 	QDir dir;
	if (dir.exists(strDestPath))
	{
		QMessageBox::information(this,tr("Information"),tr("Model name already exist."));
		return;
	}
 	if (strName == strSrcPath.section("/",-1))
 	{
 		QMessageBox::information(this,tr("Information"),tr("Cannot input same name!"));
 		return;
 	}
	BackupCate(strDestPath,strSrcPath,1);
 	//刷新表格
 	UpdateTable();
// 	ui.tableCate->setCurrentCell(0,0);
 	iSelectRow = -1;
 	//日志
	pMainFrm->Logfile.write((tr("Backup model:[%1]").arg(strSrcPath.section("/",-1))),OperationLog);

}
void WidgetManagement::slots_delete()
{
 	if (iSelectRow == iCurModelRow)
 	{
 		QMessageBox::information(this,tr("Information"),tr("Current product model is loaded, can not delete!"));
 		return;
 	}
 	if ((iSelectRow > dirList.size()-1)||iSelectRow < 0)
 	{
 		QMessageBox::information(this,tr("Information"),tr("Product model not selected!"));
 		return;
 	}
 	QString strDirPath = dirList.at(iSelectRow).absoluteFilePath();
 	if (QMessageBox::No==QMessageBox::question(this,tr("Delete"),
 		tr("Are you sure to delete:<font color=red>[%1]</font>?").arg(strDirPath.section("/",-1)),
 		QMessageBox::Yes | QMessageBox::No))
 	{
 		return;
 	}	
	DeleteCate(strDirPath);
 	UpdateTable();
 	iSelectRow = -1;
 	//日志
	pMainFrm->Logfile.write((tr("Delete model:[%1]").arg(strDirPath.section("/",-1))),OperationLog);

}
void WidgetManagement::slots_action_selectImage()
{
	QString strDirPath = dirList.at(iSelectRow).absoluteFilePath();
	QString strImagePath = strDirPath + "/Modle.bmp";

	QDir dir;
	if (dir.exists(strImagePath))
	{
		if (QMessageBox::No==QMessageBox::question(this,tr("Information"),
			tr("Image already exist.Are you sure to change it?"),
			QMessageBox::Yes | QMessageBox::No))
		{
			return;
		}	
	}

	QString filename;  
	filename = QFileDialog::getOpenFileName(this, tr("Select a image:"),"", 
		tr("Images (*.png *.bmp *.jpg *.tif *.GIF)")); //选择路径  
	if(filename.isEmpty())  
	{  
		return;  
	}
	QImage *imageSrc = new QImage(filename);
	QImage imageMin = imageSrc->scaledToHeight(80);
	if (imageMin.width()>100)
	{
		imageMin = imageMin.scaledToWidth(100);
	}
	delete imageSrc;
	imageMin.save(strImagePath);

	UpdateTable();
//	ui.tableCate->setCurrentCell(0,0);
	iSelectRow = -1;
	//日志
	pMainFrm->Logfile.write((tr("change image:[%1]").arg(strDirPath.section("/",-1))),OperationLog);

}
void WidgetManagement::DeleteCate(QString strDirPath)
{
	QDir dir(strDirPath);
	QFileInfoList fileList = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot);
	//删除文件夹内的文件
	for (int i=0;i<fileList.size();++i)
	{
		QFileInfo fileInfo = fileList.at(i);
		if (!dir.remove(fileInfo.absoluteFilePath()))
		{
			QMessageBox::information(this,tr("Information"),tr("Deleting [%1] fail!").arg(fileInfo.completeBaseName()));
			return;
		}
	}
	QFileInfoList dirList = dir.entryInfoList(QDir::AllDirs | QDir::Hidden | QDir::NoDotAndDotDot);
	//递归调用删除文件夹内的文件
	for (int i=0;i<dirList.size();++i)
	{
		DeleteCate(dirList.at(i).absoluteFilePath());
	}

	//删除该文件夹
	if (!dir.rmdir(strDirPath))
	{
		QMessageBox::information(this,tr("Information"),tr("Deleting [%1] fail!").arg(strDirPath.section('/',-1)));
		return;
	}

}
void WidgetManagement::BackupCate(QString strDestPath,QString strSrcPath,int nSure)
{
	QDir dir;
	if (!dir.exists(strDestPath))
		dir.mkpath(strDestPath);
	dir.setPath(strSrcPath);
	QFileInfoList fileList = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot);
	//复制文件夹内的文件
	for (int i=0;i<fileList.size();++i)
	{
		QFileInfo fileInfo = fileList.at(i);
		if (!QFile::copy(fileInfo.absoluteFilePath(),strDestPath+"\\"+fileInfo.completeBaseName()+"."+fileInfo.completeSuffix()))
		{
			QMessageBox::information(this,tr("Information"),tr("Copying [%1] fail!").arg(fileInfo.completeBaseName()));
			return;
		}
	}
	if(nSure)
	{
		QFileInfoList dirList = dir.entryInfoList(QDir::AllDirs | QDir::Hidden | QDir::NoDotAndDotDot);
		for (int i=0;i<dirList.size();++i)
		{
			QString strDeepDestPath = strDestPath + "/" + dirList.at(i).completeBaseName();
			QString strDeepSrcPath = strSrcPath + "/" + dirList.at(i).completeBaseName();
			BackupCate(strDeepDestPath,strDeepSrcPath,nSure);
		}
	}
}
void WidgetManagement::slots_export()
{
	QString pathName = QFileDialog::getExistingDirectory(this, tr("Export Cart"),".\\ModelInfo");
	
	if ((iSelectRow > dirList.size()-1)||iSelectRow < 0)
	{
		QMessageBox::information(this,tr("Information"),tr("Product model not selected!"));
		return;
	}

	QString strSrcPath = dirList.at(iSelectRow).absoluteFilePath();
	QString strDestPath = pathName +"\\" + dirList.at(iSelectRow).baseName() + " - Backup";
	QDir dir;
	if (dir.exists(strDestPath))
	{
		QMessageBox::information(this,tr("Information"),tr("Model already Backup."));
		return;
	}
	BackupCate(strDestPath,strSrcPath);
	//刷新表格
	UpdateTable();
	// 	ui.tableCate->setCurrentCell(0,0);
	iSelectRow = -1;
	//日志
	pMainFrm->Logfile.write((tr("Export model:[%1]").arg(strSrcPath.section("/",-1))),OperationLog);

}
void WidgetManagement::slots_import()
{
	QString pathName = QFileDialog::getExistingDirectory(this, tr("Import Cart"),".\\ModelInfo");

	QString strSrcPath = pathName;
	QString strDestPath = pMainFrm->m_sConfigInfo.m_strAppPath + "ModelInfo/" + pathName.section("\\",-1);
	QDir dir;
	if (dir.exists(strDestPath))
	{
		QMessageBox::information(this,tr("Information"),tr("Model already exist."));
		return;
	}
	// 	if (strName == strSrcPath.section("/",-1))
	// 	{
	// 		QMessageBox::information(this,tr("Information"),tr("Cannot input same name!"));
	// 		return;
	// 	}
	BackupCate(strDestPath,strSrcPath,1);
	//刷新表格
	UpdateTable();
	// 	ui.tableCate->setCurrentCell(0,0);
	iSelectRow = -1;
	//日志
	pMainFrm->Logfile.write((tr("Import model:[%1]").arg(strSrcPath.section("/",-1))),OperationLog);
}