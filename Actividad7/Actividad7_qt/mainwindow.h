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
    enum cvMats{
        CVMAT_ORIGINAL,
        CVMAT_ROJO,
        CVMAT_NARANJA,
        CVMAT_AMARILLO,
        CVMAT_RESULT,
        CVMAT_ACTUAL
    };

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_load_clicked();

    void on_btn_procesar_clicked();

    void on_btn_save_clicked();

    void on_radioBtn_rojo_clicked();

    void on_radioBtn_naranja_clicked();

    void on_radioBtn_amarillo_clicked();

    void on_radioBtn_result_clicked();

    void on_verticalSlider_minDist_valueChanged(int value);

    void on_verticalSlider_minRad_valueChanged(int value);

    void on_verticalSlider_maxRad_valueChanged(int value);

    void on_verticalSlider_minDist_sliderReleased();

    void on_verticalSlider_minRad_sliderReleased();

    void on_verticalSlider_maxRad_sliderReleased();

    void on_verticalSlider_thresh_valueChanged(int value);

    void on_verticalSilder_acc_valueChanged(int value);

    void on_verticalSlider_thresh_sliderReleased();

    void on_verticalSilder_acc_sliderReleased();

private:
    Ui::MainWindow *ui;
    cv::Mat cvMats[7];

    void update_image(cv::Mat &newMat);
    void resizeEvent(QResizeEvent *event);
    cv::Mat procesar(cv::Mat &src);
    void obtener_thresholds(cv::Mat &src, std::vector<cv::Mat> &dst);
    void resetUi();

};
#endif // MAINWINDOW_H
