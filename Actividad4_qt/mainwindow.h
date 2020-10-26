#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void set_result_pixmap(QPixmap qpixmap);
    void set_rojo_pixmap(QPixmap qpixmap);
    void set_naranja_pixmap(QPixmap qpixmap);
    void set_amarillo_pixmap(QPixmap qpixmap);

private:
    Ui::MainWindow *ui;
    QLabel* img_result;
    QLabel* img_rojo;
    QLabel* img_naranja;
    QLabel* img_amarillo;
};
#endif // MAINWINDOW_H
