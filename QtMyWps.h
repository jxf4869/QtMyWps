#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_QtMyWps.h"
#include "ChildWindow.h"
#include <QCloseEvent>
#include <QMdiSubWindow>
#include <qsignalmapper.h>
#pragma execution_character_set("utf-8")
class QtMyWps : public QMainWindow
{
    Q_OBJECT

public:
    QtMyWps(QWidget *parent = nullptr);
    ~QtMyWps();
    //初始化字体数据
    void initFontSizeBox();
    //新建文档
    void decNew();
    //打开文档
    void docOpen();
    //文件保存与另存为
    void docSave();
    //文件另存为
    void docSaveAs();
	//文档的撤销
    void docUndo();
    //文档的反撤销
    void docRedo();
    //文档剪贴
    void docCut();
    //文档复制
    void docCopy();
    //文档粘贴
    void docPaste();
    //文本加粗
    void textBold();
    //文本倾斜
    void textItalic();
    //文本下划线
    void textUnderline();
    
private:
    //所有Action都可用的功能
    void formatEnabled();
    //获取中心区是否有子窗口
    ChildWindow* actionChildWindow();
    //查找子窗口
    QMdiSubWindow* findChildWindow(const QString &docname);

private slots:
    //ui里新建文件键触发
    void on_newAction_triggered();
    //ui里关闭键触发
    void on_closeAtion_triggered();
    //ui里关闭所有
    void on_closeAllAction_triggered();
    //ui里平铺子窗口
    void on_titleAction_triggered();
    //ui里层叠子窗口
    void on_cascadeAction_triggered();
    //ui里下一个子窗口
    void on_nextAction_triggered();
    //ui里上一个子窗口
    void on_previousAction_triggered();
    //ui里打开文件
    void on_openAction_triggered();
    //ui里保存文件
    void on_saveAction_triggered();
    //ui里另存为文件
    void on_saveAsAction_triggered();
    //ui里撤销
    void on_undoAction_triggered();
    //ui里反撤销
    void on_redoAction_triggered();
    //ui里剪贴
    void on_cutAction_triggered();
    //ui里复制
    void on_copyAction_triggered();
    //ui里粘贴
    void on_pasteAction_triggered();
    //ui里加粗
    void on_boldAction_triggered();
    //ui里倾斜
    void on_italicAction_triggered();
    //ui里下划线
    void on_underLineAction_triggered();


    //刷新菜单
    void on_freshMenus();
    //菜单增加窗口对应的子项
    void on_addSubWindowListMenu();
    //设置活动按钮信号对应的窗口子部件
    void on_setActiveSubWindow(QWidget* widget);
protected:
    //重写关闭事件
    void closeEvent(QCloseEvent *event);
private:
    Ui::QtMyWpsClass  ui;
    //信号映射器
    QSignalMapper* m_WndMapper;
};
