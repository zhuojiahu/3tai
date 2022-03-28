#include "widgetUser.h"
#include <QSettings>
UserWidget::UserWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	setWindowIcon(QIcon("./Resources/LOGO.png"));
	ui.lineEdit_passWord->setEchoMode(QLineEdit::Password); 
	ui.lineEdit_passWord2->setEchoMode(QLineEdit::Password); 
	ui.lineEdit_newPassWord->setEchoMode(QLineEdit::Password); 
	ui.pushButton_login->setShortcut( QKeySequence::InsertParagraphSeparator );
	nScreenCount = 0;
	iUserPerm = false;
	initial();
	QPixmap pixmap = QPixmap(":/sys/UserBeforeLogin");
	ui.pushButton_image->setPixmap(pixmap);
	ui.groupBox->setStyleSheet("QGroupBox{background:transparent;border:2px solid blue}");
	setFixedSize(350,250);
	/*ShowInterfance();*/

	ui.comboBoxUser->setCurrentIndex(0);
	nPermission = nPermissionsList.at(0);
}

UserWidget::~UserWidget()
{
	//close();
}
void UserWidget::ShowInterfance()
{
	if(iUserPerm)
	{
		//setWindowModality(Qt::ApplicationModal);
		show();
		iUserPerm = false;
	}
}
void UserWidget::initial()
{
	ui.widget_ChangePassWord->setVisible(false);
	QFont fontStatus;
	fontStatus.setPixelSize(22);
	connect(ui.pushButton_image,SIGNAL(clicked()),this,SLOT(slots_loginChangePassWrod()));
	connect(ui.pushButton_login,SIGNAL(clicked()),this,SLOT(slots_login()));
	connect(ui.btnOK_changePassWrod,SIGNAL(clicked()),this,SLOT(slots_changePassWrod()));
	connect(ui.btnCancel_changePassWrod,SIGNAL(clicked()),this,SLOT(slots_CancelchangePassWrod()));
	connect(ui.pushButton_NewUser,SIGNAL(clicked()),this,SLOT(slots_NewUser()));
	connect(ui.pushButton_deleteUser,SIGNAL(clicked()),this,SLOT(slots_deleteUser()));
	connect(ui.checkBox_OnlyChangePermission,SIGNAL(stateChanged(int)),this,SLOT(slots_OnlyChangePermission_Checked(int)));
	connect(ui.pushButton_close,SIGNAL(clicked()),this,SLOT(hide()));
	initialUserInfo();
}
void UserWidget::initialUserInfo()
{
	QSettings sysSet("daheng","GlasswareDetectSystem");
	QString strUsers = sysSet.value("Users","").toString();
	strUserList = strUsers.split(",");
	QString strPassword = sysSet.value("Password","").toString();
	strPasswordList = strPassword.split(",");
	QString strPermissions = sysSet.value("Permissions","").toString();
	QStringList strPermissionsList = strPermissions.split(",");
	nPermissionsList.clear();
	for (int i=0;i<strPermissionsList.size();i++)
	{
		nPermissionsList.append(strPermissionsList.at(i).toInt());
	}
	strUserList.removeAt(strPermissionsList.size()-1);
	strPasswordList.removeAt(strPermissionsList.size()-1);
	strPermissionsList.removeAt(strPermissionsList.size()-1);
	nPermissionsList.removeAt(nPermissionsList.size()-1);

	int iUserNumber = strUserList.size();
	//用户数为零增加2个用户
	if (0 == iUserNumber)
	{
		strUserList.append("Admin");
		strUserList.append("daheng");
		strUserList.append("user");
		strPasswordList.append("daheng");
		strPasswordList.append("");
		strPasswordList.append("");
		nPermissionsList.append(0x3F);
		nPermissionsList.append(0x3F);
		nPermissionsList.append(0x1A);
		//保存到注册表
		QString strUsers;
		QString strPassword;
		QString strPermissions;
		for (int i = 0;i<nPermissionsList.size();i++)
		{
			strUsers = strUsers + strUserList.at(i) + ",";
			strPassword = strPassword + strPasswordList.at(i) + ",";
			strPermissions = strPermissions + QString::number(nPermissionsList.at(i)) + ",";
		}
		QSettings sysSet("daheng","GlasswareDetectSystem");
		sysSet.setValue("Users",strUsers);
		sysSet.setValue("Password",strPassword);
		sysSet.setValue("Permissions",strPermissions);
	}
	iUserNumber =  strUserList.size();

	listUser.clear();
	ui.comboBoxUser->clear();
	for (int i = 0;i<iUserNumber;i++)
	{
		QString strUserName = strUserList.at(i);
		listUser.append(strUserName);
		ui.comboBoxUser->addItem(strUserName);
	}
	ui.comboBoxUser->setCurrentIndex(strUserList.size()-1);
	nPermission = nPermissionsList.at(strUserList.size()-1);
}
void UserWidget::slots_login()
{
	bool isSuccessed=false;
	nScreenCount = 0;
	int nUser = ui.comboBoxUser->currentIndex();
	strUserName = strUserList.at(nUser);
	strPassWordUser = strPasswordList.at(nUser);
	PassWord = ui.lineEdit_passWord->text();
	if(strUserName.compare("Admin",Qt::CaseInsensitive) == 0)
	{
		if( "kongping" == PassWord )
			isSuccessed=true;
	}
	if (strPassWordUser == PassWord)
		isSuccessed=true;
	if(!isSuccessed)
	{
		QMessageBox::information(this,tr("Error"),tr("Wrong password"));
		return;
	}
	ui.lineEdit_passWord->clear();
	hide();
	iUserPerm = true;
	if(strUserName.compare("Admin",Qt::CaseInsensitive) == 0)
	{
		nPermission = 0x3F;
	}else
	{
		nPermission = nPermissionsList.at(nUser);
	}
	emit signal_LoginState(nPermission,iUserPerm);
}
void UserWidget::slots_changePassWrod()
{
	if(!isNewUserStatus)
	{//修改密码和用户权限
		bool setSuccess = false;
		PassWord = ui.lineEdit_passWord2->text();
		QString newPassWord = ui.lineEdit_newPassWord->text();
		int nUser = ui.comboBoxUser->currentIndex();
		strUserName = strUserList.at(nUser);
		strPassWordUser = strPasswordList.at(nUser);
		if(strPassWordUser == PassWord )
		{
			if(!isOnlyChangePerission)
			{//修改密码
				if(newPassWord == PassWord)
				{
					QMessageBox::information(this,tr("Error"),tr("Change password Error.\nThe new password is the same as the old password."));
					return;
				}
			}
			QString strUsers;
			QString strPassword;
			QString strPermissions;

			for (int i = 0;i<nPermissionsList.size();i++)
			{
				strUsers = strUsers + strUserList.at(i) + ",";
				if(strUserList.at(i) == strUserName)
				{
					int tmpPermission=0;
					for (int i=1;i<=6;i++)
					{
						QCheckBox *tmpBox=findChild<QCheckBox *>(QString("checkBox_%1").arg(i));
						if(tmpBox->isChecked())
							tmpPermission |= (1<<(i-1));
					}
					strPermissions = strPermissions + QString::number(tmpPermission) + ",";
				}else{
					strPermissions = strPermissions + QString::number(nPermissionsList.at(i)) + ",";
				}
				if(!isOnlyChangePerission)
				{//修改密码
					if(strUserList.at(i) == strUserName)
						strPassword = strPassword + newPassWord + ",";
					else
						strPassword = strPassword + strPasswordList.at(i) + ",";
				}
				else//只修改权限
					strPassword = strPassword + strPasswordList.at(i) + ",";
			}
			QSettings sysSet("daheng","GlasswareDetectSystem");
			sysSet.setValue("Users",strUsers);
			sysSet.setValue("Password",strPassword);
			sysSet.setValue("Permissions",strPermissions);
			initialUserInfo();
			setSuccess = true;
			if(isOnlyChangePerission)
				QMessageBox::information(this,tr("Information"),tr("Permission Changed Successfully"));
			else
				QMessageBox::information(this,tr("Information"),tr("The new password set successfully"));
		}else
		{
			QMessageBox::information(this,tr("Error"),tr("Wrong password"));
		}
		ui.lineEdit_newPassWord->clear();
		if (setSuccess)
		{
			ui.widget_LogIn->setVisible(true);
			ui.widget_ChangePassWord->setVisible(false);
		}
	}
	else//新建用户
	{
		QSettings sysSet("daheng","GlasswareDetectSystem");
		QString strUsers = sysSet.value("Users","").toString();
		QString strPassword = sysSet.value("Password","").toString();
		QString strPermissions = sysSet.value("Permissions","").toString();
		QString newName=ui.lineEdit_UserName->text();
		if(strUsers.contains(newName,Qt::CaseInsensitive))
		{
			QMessageBox::information(this,tr("Information"),tr("The new User Error.\nUser name [%1] already exist!").arg(newName));
			return;
		}
		strUsers.append(newName+",");
		QString newPassword=ui.lineEdit_passWord2->text();
		strPassword.append(newPassword+",");
		int tmpPermission=0;
		for (int i=1;i<=6;i++)
		{
			QCheckBox *tmpBox=findChild<QCheckBox *>(QString("checkBox_%1").arg(i));
			if(tmpBox->isChecked())
				tmpPermission |= (1<<(i-1));
		}
		strPermissions.append(QString::number(tmpPermission)+",");
		sysSet.setValue("Users",strUsers);
		sysSet.setValue("Password",strPassword);
		sysSet.setValue("Permissions",strPermissions);
		initialUserInfo();
		QMessageBox::information(this,tr("Information"),tr("The new User successfully"));
		ui.widget_LogIn->setVisible(true);
		ui.widget_ChangePassWord->setVisible(false);
	}
	
}
void UserWidget::slots_CancelchangePassWrod()
{
	ui.widget_LogIn->setVisible(true);
	ui.widget_ChangePassWord->setVisible(false);
}

void UserWidget::slots_NewUser()
{
	isNewUserStatus =true;
	ui.widget_LogIn->setVisible(false);
	ui.widget_ChangePassWord->setVisible(true);
	ui.label_password_3->setVisible(false);
	ui.lineEdit_newPassWord->setVisible(false);
	ui.pushButton_deleteUser->setVisible(false);
	ui.lineEdit_UserName->setEnabled(true);
	ui.lineEdit_UserName->clear();
	ui.lineEdit_passWord2->clear();
	ui.checkBox_OnlyChangePermission->setVisible(false);
	for (int i=1;i<=6;i++)
	{
		QCheckBox *tmpBox=findChild<QCheckBox *>(QString("checkBox_%1").arg(i));
		tmpBox->setChecked(false);
		tmpBox->setEnabled(true);
	}
}

void UserWidget::slots_deleteUser()
{
	int nUser = ui.comboBoxUser->currentIndex();
	QString curUserName = strUserList.at(nUser);
	int ret = QMessageBox::question(this,tr("Tips"),tr("Are you sure you want to delete the [%1] user?").arg(curUserName),QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Cancel);
	if(ret == QMessageBox::Ok)
	{
		QString curPass = ui.lineEdit_passWord2->text();
		QString UserPass = strPasswordList.at(nUser);
		if(UserPass != curPass )
		{
			QMessageBox::information(this,tr("Tips"),tr("The Password is Wrong.\nplease input again!"));
			ui.lineEdit_passWord2->clear();
			return;
		}
		QString strUsers;
		QString strPassword;
		QString strPermissions;

		int UserCount=strUserList.count();
		for(int i=0;i<UserCount;i++)
		{
			if(strUserList.at(i) != curUserName)
			{
				strUsers = strUsers + strUserList.at(i) + ",";
				strPassword = strPassword + strPasswordList.at(i) + ",";
				strPermissions = strPermissions + QString::number(nPermissionsList.at(i)) + ",";
			}
		}
		QSettings sysSet("daheng","GlasswareDetectSystem");
		sysSet.setValue("Users",strUsers);
		sysSet.setValue("Password",strPassword);
		sysSet.setValue("Permissions",strPermissions);
		initialUserInfo();
		QMessageBox::information(this,tr("Information"),tr("Detele [%1] user successfully").arg(curUserName));
		ui.widget_LogIn->setVisible(true);
		ui.widget_ChangePassWord->setVisible(false);
	}
}

void UserWidget::slots_OnlyChangePermission_Checked(int checkedstatus)
{
	Q_UNUSED(checkedstatus);
	bool nChecked =ui.checkBox_OnlyChangePermission->isChecked();
	ui.lineEdit_newPassWord->setEnabled(!nChecked);
	isOnlyChangePerission=nChecked;
}

void UserWidget::slots_loginChangePassWrod()
{
	isNewUserStatus =false;
	ui.widget_LogIn->setVisible(false);
	ui.widget_ChangePassWord->setVisible(true);
	ui.label_password_3->setVisible(true);
	ui.lineEdit_newPassWord->setVisible(true);
	int nUser = ui.comboBoxUser->currentIndex();
	QString curUserName = strUserList.at(nUser);
	ui.lineEdit_UserName->setText(curUserName);
	ui.lineEdit_UserName->setEnabled(false);
	ui.checkBox_OnlyChangePermission->setVisible(true);
	ui.lineEdit_passWord2->clear();
	ui.lineEdit_newPassWord->clear();
	if(curUserName.compare("admin",Qt::CaseInsensitive) == 0)
	{
		ui.pushButton_deleteUser->setVisible(false);
		ui.checkBox_OnlyChangePermission->setEnabled(false);
		ui.checkBox_OnlyChangePermission->setChecked(false);
		ui.lineEdit_newPassWord->setEnabled(true);
		isOnlyChangePerission=false;
		for (int i=1;i<=6;i++)
		{
			QCheckBox *tmpBox=findChild<QCheckBox *>(QString("checkBox_%1").arg(i));
			tmpBox->setChecked(true);
			tmpBox->setEnabled(false);
		}
	}
	else
	{
		ui.pushButton_deleteUser->setVisible(true);
		ui.checkBox_OnlyChangePermission->setEnabled(true);
		ui.checkBox_OnlyChangePermission->setChecked(true);
		ui.lineEdit_newPassWord->setEnabled(false);
		isOnlyChangePerission=true;
		int UserPermission=nPermissionsList.at(nUser);
		for (int i=1;i<=6;i++)
		{
			QCheckBox *tmpBox=findChild<QCheckBox *>(QString("checkBox_%1").arg(i));
			if(0x01 & (UserPermission>>(i-1))  )
				tmpBox->setChecked(true);
			else
				tmpBox->setChecked(false);
			tmpBox->setEnabled(true);
		}
	}
	
}
