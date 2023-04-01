#include "ChildWindow.h"
#include <qfileinfo.h>

ChildWindow::ChildWindow()
	
{
	//设置窗口属性:子窗口关闭销毁实例对象
	setAttribute(Qt::WA_DeleteOnClose);
	m_isSave=false;

}

ChildWindow::~ChildWindow()
{}



//获取文件名
QString ChildWindow::getCurFileName()
{
	//QString str = QFileInfo(m_CurdocPath).fileName();
	return m_CurdocPath;
}

//新建文档
void ChildWindow::newDoc()
{
	static int wndSeqNum = 1;
	m_CurdocPath = QString("我的WPS %1").arg(wndSeqNum++);
	setWindowTitle(m_CurdocPath +"[*]");
	//当前对象的文档发送文档改变信号
	//---->响应这个对象的文档修改后的槽
	connect(document(),&QTextDocument::contentsChanged,
		this,&ChildWindow::on_docBeModified);


}

void ChildWindow::on_docBeModified() {
	//设置窗体是否修改 标题加 *
	setWindowModified(document()->isModified());


}
