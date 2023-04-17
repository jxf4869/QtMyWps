#include "QtMyWps.h"
#include "ChildWindow.h"
#include <qtextedit.h>
#include <QTextLine>
#include <qfontdatabase.h>
#include <QMdiSubWindow>
#include <qfiledialog.h>
#include <qcolordialog.h>
#include <qdebug.h>

QtMyWps::QtMyWps(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	
	//设置窗体的标题
	setWindowTitle("我的WPS");
	//设置窗体的图标
	QPixmap pix("images/wps.ico");
	pix.scaled(100, 100);
	setWindowIcon(pix);
	//初始化字号选择框
	initFontSizeBox();
	//初始化多文档滚动跳(ScrollBar滚动条)
	ui.mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	ui.mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	//刷新菜单
	connect(ui.mdiArea, &QMdiArea::subWindowActivated,
		this, &QtMyWps::on_freshMenus);
	//窗口添加子窗口名称的子项
	connect(ui.windowMenu, &QMenu::aboutToShow,
		this, &QtMyWps::on_addSubWindowListMenu);
	//初始化信号映射器
	m_WndMapper = new QSignalMapper(this);
	connect(m_WndMapper, SIGNAL(mapped(QWidget*)),
		this, SLOT(on_setActiveSubWindow(QWidget*)));

	//将对齐方式活动按钮添加活动分组(保证互斥性只能选择一种)
	QActionGroup* alignGroup = new QActionGroup(this);
	alignGroup->addAction(ui.rightAlignAction);
	alignGroup->addAction(ui.leftAlignAction);
	alignGroup->addAction(ui.justifyAction);
	alignGroup->addAction(ui.centerAction);
}

QtMyWps::~QtMyWps()
{}

void QtMyWps::initFontSizeBox()
{
	QFontDatabase fontData;
	for each (int fontData in fontData.standardSizes())
	{
		ui.fontSizeComboBox->addItem(QString::number(fontData) + " 号");
	}
	//设置默认字体的大小为系统的字体大小
	QFont SysFont = QGuiApplication::font();    //拿到系统字体
	int fontSize = SysFont.pointSize();         //转化字体的点大小
	QString strFontSize = QString::number(fontSize) + " 号";//具体的字符串(字号)
	int index = ui.fontSizeComboBox->findText(strFontSize);//用获取的字符串字号查找索引
	ui.fontSizeComboBox->setCurrentIndex(index);//设置字号框当前位置
}

void QtMyWps::decNew()
{
	ChildWindow* childWnd = new ChildWindow;
	ui.mdiArea->addSubWindow(childWnd);
	//复制信号与槽连接
	connect(childWnd, &QTextEdit::copyAvailable,
		ui.cutAction, &QAction::setEnabled);
	//剪贴信号与槽连接
	connect(childWnd, &QTextEdit::copyAvailable,
		ui.copyAction, &QAction::setEnabled);

	childWnd->newDoc();
	childWnd->show();
	formatEnabled();

}
void QtMyWps::docOpen()
{
	QString docname = QFileDialog::getOpenFileName(this,
		"打开文档",
		"",
		"文本文件(*.txt);;\
                        HTML文件(*.html *.htm);;\
                        所有文件(*.*)");


	if (!docname.isEmpty()) {
		QMdiSubWindow* existWnd = findChildWindow(docname);
		if (existWnd) {
			ui.mdiArea->setActiveSubWindow(existWnd);
			return;
		}
		ChildWindow* childWnd = new ChildWindow;
		ui.mdiArea->addSubWindow(childWnd);


		//复制信号与槽连接
		connect(childWnd, &QTextEdit::copyAvailable,
			ui.cutAction, &QAction::setEnabled);
		//剪贴信号与槽连接
		connect(childWnd, &QTextEdit::copyAvailable,
			ui.copyAction, &QAction::setEnabled);

		if (childWnd->loadDoc(docname)) {
			statusBar()->showMessage("文档已打开", 3000);
			childWnd->show();
			formatEnabled();
		}
		else {
			childWnd->close();
		}
	}
}
void QtMyWps::docSave()
{
	if (actionChildWindow() &&
		actionChildWindow()->saveDoc())
	{
		//状态栏提示
		statusBar()->showMessage("保存成功", 3000);
	}
}
void QtMyWps::docSaveAs()
{
	if (actionChildWindow() &&
		actionChildWindow()->SaveAsDoc())
	{
		//状态栏提示
		statusBar()->showMessage("保存成功", 3000);
	}
}
//打印文档
void QtMyWps::docprint()
{
		//打印机像素为选中的打印机像素
		QPrinter pter(QPrinter::PrinterResolution);
		//显示打印界面
		QPrintDialog* ddlg = new QPrintDialog(&pter, this);
		if (actionChildWindow()) 
		{
			ddlg->setOption(QAbstractPrintDialog::PrintSelection, true);
		}
		//设置打印的标题
		ddlg->setWindowTitle("打印文档");
		ChildWindow* childWnd = actionChildWindow();//获取需要打印的子窗口
		if (ddlg->exec() == QDialog::Accepted) 
		{
			childWnd->print(&pter);
		}
		delete ddlg;
	
}
//打印预览
void QtMyWps::docprintPreview()
{
	QPrinter pter;
	QPrintPreviewDialog preView(&pter,this);
	connect(&preView,&QPrintPreviewDialog::paintRequested,
			this,&QtMyWps::on_printPreview);
	preView.exec();
}
void QtMyWps::docUndo()
{
	if (actionChildWindow()) 
	{
		actionChildWindow()->undo();
	}
}
void QtMyWps::docRedo()
{
	if (actionChildWindow())
	{
		actionChildWindow()->redo();
	}
}
void QtMyWps::docCut()
{
	if (actionChildWindow())
	{
		actionChildWindow()->cut();
	}
}
void QtMyWps::docCopy()
{
	if (actionChildWindow())
	{
		actionChildWindow()->copy();
	}
}
void QtMyWps::docPaste()
{
	if (actionChildWindow())
	{
		actionChildWindow()->paste();
	}
}
void QtMyWps::textBold()
{
	QTextCharFormat fmt;
	fmt.setFontWeight(ui.boldAction->isChecked() ? 
						QFont::Bold	: QFont::Normal);
	if (actionChildWindow()) 
	{
		actionChildWindow()->setFormatOnSelectedWord(fmt);
	}
}
void QtMyWps::textItalic()
{
	QTextCharFormat fmt;
	fmt.setFontItalic(ui.italicAction->isChecked());
	if (actionChildWindow())
	{
		actionChildWindow()->setFormatOnSelectedWord(fmt);
	}
}
void QtMyWps::textUnderline()
{
	QTextCharFormat fmt;
	fmt.setFontUnderline(ui.underLineAction->isChecked());
	if (actionChildWindow())
	{
		actionChildWindow()->setFormatOnSelectedWord(fmt);
	}

}
void QtMyWps::textFamily(const QString& family)
{
	QTextCharFormat fmt;
	fmt.setFontFamily(family);
	if (actionChildWindow()) 
	{
		//设置字体
		actionChildWindow()->setFormatOnSelectedWord(fmt);
	}
}
void QtMyWps::textSize(const QString& ps)
{
	QStringList strSize=ps.split(" ");
	qreal pointSize = strSize[0].toFloat();
	if (strSize[0].toFloat() > 0)
	{
		QTextCharFormat fmt;
		fmt.setFontPointSize(pointSize);

		if (actionChildWindow())
		{
			//设置字体
			actionChildWindow()->setFormatOnSelectedWord(fmt);
		}
	}
}
//文本颜色
void QtMyWps::textColor()
{
	if (actionChildWindow())
	{
		QColor color= QColorDialog::getColor(actionChildWindow()->textColor(),this);
		if (!color.isValid()) {//判断颜色合不合法
			return;
		}

		QTextCharFormat fmt;
		fmt.setForeground(color);//设置前景色
		actionChildWindow()->setFormatOnSelectedWord(fmt);

		QPixmap pix(16,16);
		pix.fill(color);
		ui.colorAction->setIcon(pix);

	}
}


void QtMyWps::paraStyle(int nStyle)
{
	if (actionChildWindow()) 
	{
		actionChildWindow()->setParaSyle(nStyle);
	}

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

void QtMyWps::on_openAction_triggered()
{
	docOpen();
}

void QtMyWps::on_saveAction_triggered()
{
	docSave();
}

void QtMyWps::on_saveAsAction_triggered()
{
	docSaveAs();
}

void QtMyWps::on_undoAction_triggered()
{
	docUndo();
}

void QtMyWps::on_redoAction_triggered()
{
	docRedo();
}

void QtMyWps::on_cutAction_triggered()
{
	docCut();
}

void QtMyWps::on_copyAction_triggered()
{
	docCopy();
}

void QtMyWps::on_pasteAction_triggered()
{
	docPaste();
}

void QtMyWps::on_boldAction_triggered()
{
	textBold();
}

void QtMyWps::on_italicAction_triggered()
{
	textItalic();
}

void QtMyWps::on_underLineAction_triggered()
{
	textUnderline();
}

void QtMyWps::on_fontComboBox_activated(const QString& argl)
{
	textFamily(argl);
}

void QtMyWps::on_fontSizeComboBox_activated(const QString& argl)
{
	textSize(argl);
}

void QtMyWps::on_leftAlignAction_triggered()
{
	if (actionChildWindow()) 
	{
		actionChildWindow()->setAlignofDocumentText(1);
	}
}

void QtMyWps::on_rightAlignAction_triggered()
{
	if (actionChildWindow())
	{
		actionChildWindow()->setAlignofDocumentText(2);
	}
}

void QtMyWps::on_justifyAction_triggered()
{
	if (actionChildWindow())
	{
		actionChildWindow()->setAlignofDocumentText(4);
	}
}

void QtMyWps::on_centerAction_triggered()
{
	if (actionChildWindow())
	{
		actionChildWindow()->setAlignofDocumentText(3);
	}
}

void QtMyWps::on_colorAction_triggered()
{
	textColor();
}

void QtMyWps::on_symbolComboBox_activated(int index)
{
	paraStyle(index);
}

void QtMyWps::on_pintAction_triggered()
{
	docprint();
}

void QtMyWps::on_pintPreviewAction_triggered()
{
	docprintPreview();
}


//刷新菜单(保存/退出/..../下一个功能可以使用)
void QtMyWps::on_freshMenus()
{
	//判断中心区有无子窗口
	bool hasSubWindow = false;
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
	ui.colorAction->setEnabled(hasSubWindow);
	
	
	//打开文档且有内容
	bool hasDocContent = (actionChildWindow() &&
					(actionChildWindow()->textCursor().hasSelection()));
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
	QMdiSubWindow* subWindow = ui.mdiArea->activeSubWindow();
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
QMdiSubWindow* QtMyWps::findChildWindow(const QString& docname)
{
	QString fliename = QFileInfo(docname).canonicalFilePath();
	for each (QMdiSubWindow * subwind in ui.mdiArea->subWindowList())
	{
		ChildWindow* childWnd = qobject_cast<ChildWindow*>(subwind->widget());
		if (childWnd->m_CurdocPath == fliename) {
			return subwind;
		}
	}
	return 0;
	
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

	for (int i = 0; i < wnds.size(); i++)
	{
		ChildWindow* childWnd = dynamic_cast<ChildWindow*>(wnds.at(i)->widget());
		QString strWindName = QString("%1 %2")
			.arg(i + 1)
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
			m_WndMapper, SLOT(map()));//map发射maped(widget*)
	//对菜单栏里的活动项设置信号映射器并指定参数
		m_WndMapper->setMapping(addAction, wnds.at(i));
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
	QMdiSubWindow* ActionSubWindow = dynamic_cast<QMdiSubWindow*>(widget);
	//设置中心部件的子窗口设置为映射过来的窗口
	ui.mdiArea->setActiveSubWindow(ActionSubWindow);
}

void QtMyWps::on_printPreview(QPrinter* printer)
{
	actionChildWindow()->print(printer);
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


