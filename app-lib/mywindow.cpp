#include "mywindow.h"
#include "ui_mywindow.h"
#include <QPixmap>
#include <QDebug>

MyWindow::MyWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MyWindow)
{
    ui->setupUi(this);
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
    font.setFamily(u8"微软雅黑");
    font.setPixelSize(50);
    ui->plainTextEdit->setFont(font);
    ui->plainTextEdit->setPlainText(u8"这是一段中文");
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
