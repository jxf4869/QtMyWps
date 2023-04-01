#pragma once
#include<qtextdocument.h>
#include <QTextEdit>
#pragma execution_character_set("utf-8")

/*
*用于创建
*/
class ChildWindow  : public QTextEdit
{
	Q_OBJECT

public:
	ChildWindow();
	~ChildWindow();

	
	
	//新建文档
	void newDoc();
    //从文件路径获取文件名
	QString getCurFileName();
protected slots:
	void on_docBeModified();
private:
	QString m_CurdocPath;	//文件名
	bool m_isSave;			//文档是否保存
};
