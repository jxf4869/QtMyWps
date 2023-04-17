/*************************************************
* Ⅰ.著作权: 奇牛学院-janny
* Ⅱ.作者: janny	版本: 1.1	完成时间: 2023/4/15
* Ⅲ.描述信息:	
*		用于管理与实现WPS子窗口的管理与功能的实现
* Ⅳ.其他内容:
*		无
* Ⅴ.主要功能列表:
*		1 新建文档
*		2 从文件路径获取文件名
*		3 加载文档
*		4 设置当前文档
*		5 保存文档
*		6 文档另存为
*		7 真正保存文档
*		8 设置格式
*		9 设置对齐
*		10设置项目符号
* Ⅵ.修改历史:
*	<作者>	<修改时间>	<版本>	<描述>
*	janny 	 --/--/--    1.0   项目开发者
*	jxf		 23/04/15	 1.1   项目学习者
**************************************************/
#pragma once
#include<qtextdocument.h>
#include <QTextEdit>
#pragma execution_character_set("utf-8")
class ChildWindow  : public QTextEdit
{
	Q_OBJECT

public:
	ChildWindow();
	~ChildWindow(); 
	
	/*---函数介绍---
	* 函数名:newDoc();
	* 描述:用于新建新建文档
	* 被调用函数:
	* 被访问表:
	* 被修改表:
	* 输入参数:
	* 输出参数:
	* 返回值:
	* 其他:
	*/
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
	//设置对齐
	void setAlignofDocumentText(int alignType);
	//设置项目符号
	void setParaSyle(int pStyle);

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
