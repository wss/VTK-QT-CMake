#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkLight.h>
#include <vtkCamera.h>
#include <vtkAutoInit.h>
#include <QVTKOpenGLNativeWidget.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//VTK_MODULE_INIT(vtkRenderingOpenGL2);
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void cylinder();
private:
    Ui::MainWindow *ui;
    QVTKOpenGLNativeWidget* m_nativeWidget;
};
#endif // MAINWINDOW_H
