#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
#include <vtkImageActor.h>
#include <vtkBMPReader.h>
#include <vtkImageLuminance.h>
#include <vtkImageMapper3D.h>
#include <vtkAutoInit.h>
#include <QVTKOpenGLWidget.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    void color2gray();

private:
    Ui::MainWindow *ui;
    QVTKOpenGLWidget* m_vtkOpenGLWidget;
    vtkSmartPointer<vtkRenderer> m_originalRenderer;
    vtkSmartPointer<vtkRenderer> m_shiftscaleRenderer;
};
#endif // MAINWINDOW_H
