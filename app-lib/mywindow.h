#ifndef MYWINDOW_H
#define MYWINDOW_H

#include "global.h"
#include <QMainWindow>
#include <QPlainTextEdit>

namespace Ui
{
    class MyWindow;
}
class QMouseEvent;
class QDragEnterEvent;
class APP_LIB_EXPORT ZPlainTextEdit : public QPlainTextEdit
{
public:
    ZPlainTextEdit(QWidget* parent = nullptr, bool isDragTextEnabled = false);
protected:
    void mousePressEvent(QMouseEvent* e) override;

    bool isDragTextEnabled;
};
class APP_LIB_EXPORT MyWindow : public QMainWindow
{
    Q_OBJECT

public:
    static QMainWindow* createGraphicsWindow(double scale = 1E7);
    explicit MyWindow(QWidget *parent = nullptr);
    ~MyWindow();

private slots:
    void on_pushButton_Reset_clicked();

    void on_pushButton_Clear_clicked();

    void on_checkBox_Underline_clicked(bool checked);

    void on_checkBox_Italic_clicked(bool checked);

    void on_checkBox_Bold_clicked(bool checked);

    void do_FontColor();

private:
    Ui::MyWindow *ui;
};

#endif // MYWINDOW_H
