#include "ChildWindow.h"
#include <qfileinfo.h>
#include <qfiledialog.h>
#include <qtextdocumentwriter.h>
#include <qmessagebox.h>
#include <QCloseEvent>
ChildWindow::ChildWindow()

{
	//设置窗口属性:子窗口关闭销毁实例对象
	setAttribute(Qt::WA_DeleteOnClose);
	m_isSave = false;

}

ChildWindow::~ChildWindow()
{}



//获取文件名
QString ChildWindow::getCurFileName()
{
	return QFileInfo(m_CurdocPath).fileName();
}

bool ChildWindow::loadDoc(const QString& docName)
{
	if (!docName.isEmpty()) {
		QFile file(docName);
		if (!file.exists())//文件不存在 
		{
			return false;
		}
		if (!file.open(QIODevice::ReadOnly)) //以只读打开文件失败
		{
			return false;
		}
		else
		{
			QByteArray byteData = file.readAll();
			if (Qt::mightBeRichText(byteData))//数据可能为富文本
			{
				setHtml(byteData);//数据设置成Html格式
			}
			else {
				setPlainText(byteData);//数据设置成纯文本
			}
			

		}
		//设置文本名及改动标识
		setCurDoc(docName);
		connect(document(), SIGNAL(contentsChanged()),
			this, SLOT(docBeModfild()));
		return true;


	}

}

void ChildWindow::setCurDoc(const QString& docName)
{
	//去除文件的路径(返回绝对路径)
	m_CurdocPath = QFileInfo(docName).canonicalFilePath();
	m_isSave = true;
	document()->setModified(false);//行编辑器的内容是否已被用户修改
	setWindowModified(false);// 窗口中显示的文档是否有未保存的更改(不显示改动标识*)
	setWindowTitle(getCurFileName() + "[*]");//设置子窗口标题

}
//保存文档
bool ChildWindow::saveDoc()
{
	if (m_isSave)//已经保存了文档
	{
		return SaveDocOpt(m_CurdocPath);
	}
	else {//没有保存文档(另存为)
		SaveAsDoc();
	}
	return false;
}
//文档另存为
bool ChildWindow::SaveAsDoc()
{
	//获取保存文件的名字
	QString docName = QFileDialog::getSaveFileName(this,
		"另存为",
		m_CurdocPath,
		"HTML文档(*.html *htm);;"
		"所有文件(*.*)");
	if (docName.isEmpty())//保存的文档名为空 
	{
		return false;//保存失败

	}
	else {

		return SaveDocOpt(docName);
	}
}
//具体保存文档操作
bool ChildWindow::SaveDocOpt(QString docName)
{	//判断文件名后缀是否为.htm或html结尾(大小写不敏感)
	if (!(docName.endsWith(".htm", Qt::CaseInsensitive) ||
		docName.endsWith(".html", Qt::CaseInsensitive)))
	{
		docName = docName + ".html";
	}
	QTextDocumentWriter writer(docName);//创建文档写入对象
	bool isSuccess = writer.write(this->document());
	if (isSuccess)//写入文档成功
	{
		setCurDoc(docName);
	}
	return isSuccess;
}

void ChildWindow::setFormatOnSelectedWord(const QTextCharFormat& fmt)
{
	//获取文档当前光标
	QTextCursor  tcursor = textCursor();
	if (!tcursor.hasSelection())//光标是否选中文本
	{
		tcursor.select(QTextCursor::WordUnderCursor);
	}
	//合并文本格式
	tcursor.mergeCharFormat(fmt);
	mergeCurrentCharFormat(fmt);
}
void ChildWindow::closeEvent(QCloseEvent * event)
{
	if (promptSave()) 
	{
		event->accept();//接受此事件
	}
	else 
	{
		event->ignore();
	}
}

bool ChildWindow::promptSave()
{
	if (!document()->isModified()) //文档没有修改
	{
		return true;
	}

	QMessageBox::StandardButton result;
	result = QMessageBox::warning(this,
		"系统提示",
		QString("是否保存已修改的%1").arg(getCurFileName()),
		QMessageBox::Yes | QMessageBox::No | QMessageBox::Discard);

	if (result== QMessageBox::Yes)
	{
		return saveDoc();
	}
	else if(result == QMessageBox::No)
	{
		return false;
	}
	else if (result == QMessageBox::Discard)
	{
		return true;
	}
}



//新建文档
void ChildWindow::newDoc()
{
	static int wndSeqNum = 1;
	m_CurdocPath = QString("我的WPS %1").arg(wndSeqNum++);
	setWindowTitle(m_CurdocPath + "[*]");
	//当前对象的文档发送文档改变信号
	//---->响应这个对象的文档修改后的槽
	connect(document(), &QTextDocument::contentsChanged,
		this, &ChildWindow::on_docBeModified);


}

void ChildWindow::on_docBeModified() {
	//设置窗体是否修改 标题加 *
	setWindowModified(document()->isModified());


}
