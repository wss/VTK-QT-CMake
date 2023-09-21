#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vtkSmartPointer.h>
#include <vtkImageViewer2.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkBMPReader.h>
#include <vtkImageData.h>
#include <vtkImageIterator.h>
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
    void changeImagePixel();

private:
    Ui::MainWindow *ui;
    QVTKOpenGLWidget* m_vtkOpenGlwidget;
    vtkSmartPointer<vtkBMPReader> m_bmpReader;
    vtkSmartPointer<vtkImageViewer2> m_vtkImageViewer2;

};
#endif // MAINWINDOW_H
