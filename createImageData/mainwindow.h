#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVTKOpenGLWidget.h>
#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkInformation.h>
#include <vtkImageActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyle.h>
#include <vtkAutoInit.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void createImageData();
private:
    Ui::MainWindow *ui;
    QVTKOpenGLWidget* m_vtkOpenGLWidget;
};
#endif // MAINWINDOW_H
