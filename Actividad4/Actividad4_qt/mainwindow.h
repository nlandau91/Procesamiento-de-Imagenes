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
enum cvMats{
    CVMAT_ORIGINAL,
    CVMAT_ROJO,
    CVMAT_NARANJA,
    CVMAT_AMARILLO,
    CVMAT_ACTUAL
};

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_load_clicked();

    void on_radioBtn_rojo_clicked();

    void on_radioBtn_naranja_clicked();

    void on_radioBtn_amarillo_clicked();

    void on_radioBtn_original_clicked();

    void on_slider_kSize1_sliderMoved(int position);

    void on_slider_pixels_sliderMoved(int position);

    void on_slider_kSize2_sliderMoved(int position);

    ;

    void on_comboBox_currentIndexChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    cv::Mat cvMats[7];
    int pixPerCookie;
    int kSize1;
    int kSize2;
    int shape;


    void init_values();
    void update_image();
    void procesar(cv::Mat &src, cv::Mat &dst);
    void obtener_thresholds(cv::Mat &src);
    void calcular_cookies();
    void resizeEvent(QResizeEvent* event);
};
#endif // MAINWINDOW_H
