#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_QtMyWps.h"
#include "ChildWindow.h"
#include <QCloseEvent>
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
    
         
    
private:
    //所有Action都可用的功能
    void formatEnabled();
    //获取中心区是否有子窗口
    ChildWindow* actionChildWindow();
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
