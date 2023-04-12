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
	//加载文档
	bool loadDoc(const QString &docName);
	//设置当前文档
	void setCurDoc(const QString &docName);
	//保存文档
	bool saveDoc();
	//文档另存为
	bool SaveAsDoc();
	//真正保存文档
	bool SaveDocOpt(QString docName);
	//设置格式
	void setFormatOnSelectedWord(const QTextCharFormat &fmt);
	


	QString m_CurdocPath;	//文件路径
protected:
	//重写关闭事件
	void closeEvent(QCloseEvent* event);
private:
	bool promptSave();
protected slots:
	void on_docBeModified();
private:
	
	
	bool m_isSave;			//文档是否保存
};
