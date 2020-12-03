#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QResizeEvent>
#include <opencv2/opencv.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_load_clicked();

    void on_btn_procesar_clicked();

    void on_btn_save_clicked();

private:
    Ui::MainWindow *ui;
    cv::Mat src;
    cv::Mat result;

    void update_image();
    void resizeEvent(QResizeEvent *event);

};
#endif // MAINWINDOW_H
