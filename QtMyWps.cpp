#include "QtMyWps.h"
#include "ChildWindow.h"
#include <qtextedit.h>
#include <QTextLine>
#include <qfontdatabase.h>
#include <QMdiSubWindow>
QtMyWps::QtMyWps(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    //设置窗体的标题
    setWindowTitle("我的WPS");
    //设置窗体的图标
    QPixmap pix("images/wps.ico");
    pix.scaled(100,100);
    setWindowIcon(pix);
    //初始化字号选择框
    initFontSizeBox();
    //初始化多文档滚动跳(ScrollBar滚动条)
    ui.mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui.mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    //刷新菜单
    connect(ui.mdiArea,&QMdiArea::subWindowActivated,
             this,&QtMyWps::on_freshMenus);
    //窗口添加子窗口名称的子项
    connect(ui.windowMenu,&QMenu::aboutToShow,
        this,&QtMyWps::on_addSubWindowListMenu);
    //初始化信号映射器
    m_WndMapper = new QSignalMapper(this);
    connect(m_WndMapper,SIGNAL(mapped(QWidget*)),
           this,SLOT(on_setActiveSubWindow(QWidget*)));
}

QtMyWps::~QtMyWps()
{}

void QtMyWps::initFontSizeBox()
{
    QFontDatabase fontData;
    for each (int fontData in fontData.standardSizes())
    {
        ui.fontSizeComboBox->addItem(QString::number(fontData)+"号");
    }
    //设置默认字体的大小为系统的字体大小
    QFont SysFont = QGuiApplication::font();    //拿到系统字体
    int fontSize = SysFont.pointSize();         //转化字体的点大小
    QString strFontSize = QString::number(fontSize)+"号";//具体的字符串(字号)
    int index = ui.fontSizeComboBox->findText(strFontSize);//用获取的字符串字号查找索引
    ui.fontSizeComboBox->setCurrentIndex(index);//设置字号框当前位置
}

void QtMyWps::decNew()
{
    ChildWindow* childWnd = new ChildWindow;
    ui.mdiArea->addSubWindow(childWnd);
    //复制信号与槽连接
    connect(childWnd, &QTextEdit::copyAvailable,
        ui.cutAction,&QAction::setEnabled);
    //剪贴信号与槽连接
    connect(childWnd, &QTextEdit::copyAvailable,
        ui.copyAction, &QAction::setEnabled);
   
    childWnd->newDoc();
    childWnd->show();
    formatEnabled();
        
}
//ui里关闭活动子窗口
void QtMyWps::on_closeAtion_triggered()
{
    ui.mdiArea->closeActiveSubWindow();
}
//ui里关闭所有子窗口
void QtMyWps::on_closeAllAction_triggered()
{
    ui.mdiArea->closeAllSubWindows();
}
//ui里所有子窗口平铺
void QtMyWps::on_titleAction_triggered()
{
    ui.mdiArea->tileSubWindows();
}
//ui里所有子窗口层叠

void QtMyWps::on_cascadeAction_triggered()
{
    ui.mdiArea->cascadeSubWindows();
}
//ui里下一个子窗口
void QtMyWps::on_nextAction_triggered()
{
    ui.mdiArea->activateNextSubWindow();
}
//ui里上一个子窗口
void QtMyWps::on_previousAction_triggered()
{
    ui.mdiArea->activatePreviousSubWindow();
}



//刷新菜单(保存/退出/..../下一个功能可以使用)
void QtMyWps::on_freshMenus()
{
    //判断中心区有无子窗口
    bool hasSubWindow =false;
    hasSubWindow = (actionChildWindow() != nullptr);
    ui.saveAction->setEnabled(hasSubWindow);        //保存
    ui.saveAsAction->setEnabled(hasSubWindow);      //全部保存
    ui.pintAction->setEnabled(hasSubWindow);        //打印
    ui.pintPreviewAction->setEnabled(hasSubWindow); //打印预览
    ui.pasteAction->setEnabled(hasSubWindow);       //粘贴
    ui.closeAtion->setEnabled(hasSubWindow);        //关闭
    ui.closeAllAction->setEnabled(hasSubWindow);    //关闭所有
    ui.titleAction->setEnabled(hasSubWindow);       //平铺
    ui.cascadeAction->setEnabled(hasSubWindow);     //层叠
    ui.nextAction->setEnabled(hasSubWindow);        //下一步
    ui.previousAction->setEnabled(hasSubWindow);    //前一步

    //打开文档且有内容
    bool hasDocContent = actionChildWindow() && (actionChildWindow()->textCursor().hasSelection());
    //加粗
    ui.boldAction->setEnabled(hasDocContent);
    //倾斜
    ui.italicAction->setEnabled(hasDocContent);
    //下划线
    ui.underLineAction->setEnabled(hasDocContent);
    //左对齐
    ui.leftAlignAction->setEnabled(hasDocContent);
    //右对齐
    ui.rightAlignAction->setEnabled(hasDocContent);
    //二端对齐
    ui.justifyAction->setEnabled(hasDocContent);
    //居中对齐
    ui.centerAction->setEnabled(hasDocContent);
    //剪贴
    ui.cutAction->setEnabled(hasDocContent);
    //复制
    ui.copyAction->setEnabled(hasDocContent);
    //颜色
    ui.colorAction->setEnabled(hasDocContent);
}



void QtMyWps::formatEnabled()
{
    //加粗
    ui.boldAction->setEnabled(true);
    //倾斜
    ui.italicAction->setEnabled(true);
    //下划线
    ui.underLineAction->setEnabled(true);
    //左对齐
    ui.leftAlignAction->setEnabled(true);
    //右对齐
    ui.rightAlignAction->setEnabled(true);
    //二端对齐
    ui.justifyAction->setEnabled(true);
    //居中对齐
    ui.centerAction->setEnabled(true);
}

//获取中心部件子窗口
ChildWindow* QtMyWps::actionChildWindow()
{
    QMdiSubWindow* subWindow= ui.mdiArea->activeSubWindow();
    if (subWindow) 
    {  
        ChildWindow* childWindow = dynamic_cast<ChildWindow*>(subWindow->widget());
        return childWindow;
    }
    else
    {
        return nullptr;
    }


   
}
void QtMyWps::on_newAction_triggered() 
{
    decNew();
}

void QtMyWps::on_addSubWindowListMenu()
{
    ui.windowMenu->clear();
    ui.windowMenu->addAction(ui.closeAtion);
    ui.windowMenu->addAction(ui.closeAllAction);
    ui.windowMenu->addSeparator();
    ui.windowMenu->addAction(ui.titleAction);
    ui.windowMenu->addAction(ui.cascadeAction);
    ui.windowMenu->addSeparator();
    ui.windowMenu->addAction(ui.nextAction);
    ui.windowMenu->addAction(ui.previousAction);
    

    QList <QMdiSubWindow*> wnds;
    wnds = ui.mdiArea->subWindowList();
    if (!wnds.isEmpty())
    {
        ui.windowMenu->addSeparator();
    }

    for (int i=0;i<wnds.size();i++) 
    {
        ChildWindow* childWnd = dynamic_cast<ChildWindow*>(wnds.at(i)->widget());
        QString strWindName = QString("%1 %2")
            .arg(i+1)
            .arg(childWnd->getCurFileName());
        QAction* addAction = new QAction(strWindName);
        ui.windowMenu->addAction(addAction);
        //等于上面代码
        //QAction* addAction =  ui.windowMenu->addAction(strWindName);
        addAction->setCheckable(true);
        //用当前遍历的窗口与活动的窗口相比设置是否选中
        addAction->setChecked(childWnd == actionChildWindow());
        //菜单里的Action被激发通过信号映射器响应中心部件的子窗口
        connect(addAction, SIGNAL(triggered(bool)),
                m_WndMapper,SLOT(map()));//map发射maped(widget*)
        //对菜单栏里的活动项设置信号映射器并指定参数
        m_WndMapper->setMapping(addAction,wnds.at(i));
    }
    formatEnabled();
}

void QtMyWps::on_setActiveSubWindow(QWidget* widget)
{
    if (!widget)//子窗口部件无效
    {
        return;
    }
    //把映射器上的部件转化成子窗口
    QMdiSubWindow* ActionSubWindow = dynamic_cast<QMdiSubWindow*>( widget);
    //设置中心部件的子窗口设置为映射过来的窗口
    ui.mdiArea->setActiveSubWindow(ActionSubWindow);
}

void QtMyWps::closeEvent(QCloseEvent* event)
{   
    //关闭所有子窗口
    ui.mdiArea->closeAllSubWindows();
    
    if (ui.mdiArea->currentSubWindow()) {//还有子窗口
        //忽略此事件
        event->ignore();
    
    }
    else//没有子窗口
    {
        //接受此事件
        event->accept();
    }
}


