#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

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

    void on_radioBtn_result_clicked();

    void on_radioBtn_rojo_clicked();

    void on_radioBtn_naranja_clicked();

    void on_radioBtn_amarillo_clicked();

private:
    Ui::MainWindow *ui;
    cv::Mat original;
    cv::Mat result;
    cv::Mat rojo;
    cv::Mat naranja;
    cv::Mat amarillo;
    cv::Mat kernel;
    int pixPerCookie;


    void init_values();
    void update_image(cv::Mat src);
};
#endif // MAINWINDOW_H
