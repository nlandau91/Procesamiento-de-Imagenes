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
        CVMAT_ROSA,
        CVMAT_NARANJA,
        CVMAT_AMARILLO,
        CVMAT_CHOCOLATE,
        CVMAT_RESULT,
        CVMAT_ACTUAL
    };
    enum cvCircles{
        CVCIRCLE_ROSA,
        CVCIRCLE_NARANJA,
        CVCIRCLE_AMARILLO,
        CVCIRCLE_CHOCOLATE
    };
    enum cvColors{
        CVCOLOR_ROSA,
        CVCOLOR_NARANJA,
        CVCOLOR_AMARILLO,
        CVCOLOR_CHOCOLATE
    };

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_load_clicked();

    void on_btn_procesar_clicked();

    void on_btn_save_clicked();

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

    void on_radioBtn_chocolate_clicked();

    void on_radioBtn_rosa_clicked();

    void on_verticalSlider_dp_sliderReleased();

    void on_verticalSlider_dp_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    cv::Mat cvMats[7];
    std::vector<cv::Vec3f> cvCircles[4];
    std::vector<cv::Point> rotas;
    cv::Scalar cvColors[4] = {cv::Scalar(181,113,255,255),cv::Scalar(16,64,180,255),cv::Scalar(0,255,255,255),cv::Scalar(18,52,98,255)};

    void update_image(cv::Mat &newMat);
    void resizeEvent(QResizeEvent *event);
    cv::Mat procesar(cv::Mat &src);
    void resetUi();

};
#endif // MAINWINDOW_H
