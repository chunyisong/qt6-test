#include "mywindow.h"
#include "ui_mywindow.h"
#include <QMouseEvent>
#include <QPixmap>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QPushButton>
#include <QVBoxLayout>

ZPlainTextEdit::ZPlainTextEdit(QWidget* parent, bool isDragTextEnabled_)
    : QPlainTextEdit(parent), isDragTextEnabled(isDragTextEnabled_)
{
}
void ZPlainTextEdit::mousePressEvent(QMouseEvent* evt) {
    if (!isDragTextEnabled && evt->buttons() == Qt::LeftButton && evt->modifiers() == Qt::NoModifier) {
        const auto& txtCursor = textCursor();
        if (txtCursor.hasSelection()) {
            moveCursor(QTextCursor::Start); // 避免鼠标按压在选中的文本上触发拖动导致exec阻塞
        //     auto tcp = cursorForPosition(evt->position().toPoint()).position();
        //     qDebug() << __FUNCTION__ << "#" << __LINE__ << u8" wasm 为避免拖拽动作鼠标按下时将取消已选择的文本:"
        //         << txtCursor.selectedText() << ",cursorRect:" << cursorRect() << ",pos:" << evt->position()
        //         << "s0:" << txtCursor.selectionStart() << ",s1:" << txtCursor.selectionEnd() << ",es:" << tcp;
        //     if (tcp <= txtCursor.selectionEnd() && tcp >= txtCursor.selectionStart()) {
        //         txtCursor.clearSelection();
        //         setTextCursor(txtCursor);
        //     }
        }
    }
    QPlainTextEdit::mousePressEvent(evt);
}

MyWindow::MyWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MyWindow)
{
    ui->setupUi(this);
    auto oldEdit = ui->plainTextEdit;
    ui->plainTextEdit = new ZPlainTextEdit(this, false);
    // wasm 在输入框选择的文本之上按下鼠标时导致无尽事件循环导致内存宿主机耗尽
    qDebug() << __FUNCTION__ << "#" << __LINE__ << u8" wasm 在QTextEdit输入框选择的文本之上按下鼠标时导致无尽事件循环导致内存宿主机耗尽";
    auto layoutItem = ui->verticalLayout->replaceWidget(oldEdit, ui->plainTextEdit);
    delete layoutItem;
    delete oldEdit;
    this->on_pushButton_Reset_clicked();
    QObject::connect(ui->radioButton_Blue, &QRadioButton::clicked, this, qOverload<>(&MyWindow::do_FontColor));
    QObject::connect(ui->radioButton_Red, &QRadioButton::clicked, this, qOverload<>(&MyWindow::do_FontColor));
    QObject::connect(ui->radioButton_Black, &QRadioButton::clicked, this, qOverload<>(&MyWindow::do_FontColor));
    ui->img_label->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->img_label->setText("右键\n换图");
    ui->img_label->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    ui->img_label->setAlignment(Qt::AlignBottom | Qt::AlignCenter);
    QObject::connect(ui->img_label, &QLabel::customContextMenuRequested, this, [this](const auto& point) {
        QString img = ui->img_label->property("img").toString();
        if (img.startsWith(":/lib2")) {
            img = ":/app/res/en.png";
        }
        else if (img.startsWith(":/lib")) {
            img = ":/lib2/res/en2.png";
        }
        else {
            img = ":/lib/res/en.png";
        }
        ui->img_label->setProperty("img",img);
        ui->img_label->setPixmap(img);
        qDebug() << __FUNCTION__ << "#" << __LINE__ << " QLabel::customContextMenuRequested point:" << point << ",img:" << img;
    });
}

MyWindow::~MyWindow()
{
    delete ui;
}

void MyWindow::on_pushButton_Reset_clicked()
{
    QFont font = ui->plainTextEdit->font();
    // font.setFamily(u8"微软雅黑");
    font.setPixelSize(50);
    ui->plainTextEdit->setFont(font);
    ui->plainTextEdit->setPlainText(u8"Qt6.8 支持中文输入/复制/粘贴/提示框/消息框了!");
    qDebug() << __FUNCTION__ << "#" << __LINE__ << " Reset";
}

void MyWindow::on_pushButton_Clear_clicked()
{
    ui->plainTextEdit->clear();
}

void MyWindow::on_checkBox_Underline_clicked(bool checked)
{
    QFont font = ui->plainTextEdit->font();
    font.setUnderline(checked);
    ui->plainTextEdit->setFont(font);
    qDebug() << __FUNCTION__ << "#" << __LINE__ << " Reset";
}

void MyWindow::on_checkBox_Italic_clicked(bool checked)
{
    QFont font = ui->plainTextEdit->font();
    font.setItalic(checked);
    ui->plainTextEdit->setFont(font);
}

void MyWindow::on_checkBox_Bold_clicked(bool checked)
{
    QFont font = ui->plainTextEdit->font();
    font.setBold(checked);
    ui->plainTextEdit->setFont(font);
}

void MyWindow::do_FontColor()
{
    QPalette plet = ui->plainTextEdit->palette();
    if (ui->radioButton_Red->isChecked())
    {
        plet.setColor(QPalette::Text, Qt::red);
    }
    else if (ui->radioButton_Blue->isChecked())
    {
        plet.setColor(QPalette::Text, Qt::blue);
    }
    else if (ui->radioButton_Black->isChecked())
    {
        plet.setColor(QPalette::Text, Qt::black);
    }
    ui->plainTextEdit->setPalette(plet);
}

QMainWindow* MyWindow::createGraphicsWindow(double scale){
    auto win = new QMainWindow;
    auto scene = new QGraphicsScene(win);
    scene->setSceneRect(QRectF(0, 0, 5000, 5000));
    auto view = new QGraphicsView(scene);
    auto fitBtn = new QPushButton("FitInViewBtn");
    fitBtn->setToolTip("FitInViewBtn:big scale greater than 1E106 will move item far away from view port");
    auto layout = new QVBoxLayout;
    layout->addWidget(fitBtn);
    layout->addWidget(view);
    QWidget *widget = new QWidget;
    widget->setLayout(layout);
    win->setCentralWidget(widget);
    win->setGeometry(100, 100, 800, 600);
    win->show();
    auto textItem = new QGraphicsTextItem("TestTextItem");
    textItem->setFlag(QGraphicsItem::ItemIsMovable);
    textItem->setFlag(QGraphicsItem::ItemIsSelectable);
    textItem->setPos(view->mapToScene(view->viewport()->rect().center()));
    scene->addItem(textItem);

    // scale number to chang item view size,bigger number greater than 1E106 will move item far away from view port
    if (scale < 1E-10 || scale > 1E10) {
        scale = 1.0E7;
    }
    fitBtn->connect(fitBtn,&QPushButton::clicked,[=](){
        auto extent = textItem->sceneBoundingRect();
        qDebug() << "before>> extent:" << extent << ",extentCenter:" << extent.center() << ",viewRect:" << view->viewport()->rect()
                 << ",viewSceneCenter:" << view->mapToScene(view->viewport()->rect().center())
                 << ",view.m11:" << view->transform().m11() << ",item.m11:" << textItem->transform().m11();        textItem->setScale(1/scale);

        textItem->setScale(1/scale);
        extent = textItem->sceneBoundingRect();

        view->fitInView(extent,Qt::KeepAspectRatio);

        qDebug() << "after>> extent:" << extent << ",extentCenter:" << extent.center() << ",viewRect:" << view->viewport()->rect()
                 << ",viewSceneCenter:" << view->mapToScene(view->viewport()->rect().center())
                 << ",view.m11:" << view->transform().m11() << ",item.m11:" << textItem->transform().m11();
    });
    return win;
}