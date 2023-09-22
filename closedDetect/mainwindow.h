#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVTKOpenGLWidget.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class vtkPolyData;
class vtkRenderer;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    void closedDetect();
    void generateData(vtkSmartPointer<vtkPolyData> input);
private:
    Ui::MainWindow *ui;
    QVTKOpenGLWidget* m_vtkOpenGLWidget;
    vtkSmartPointer<vtkRenderer> m_leftRenderer;
    vtkSmartPointer<vtkRenderer> m_rightRenderer;
};
#endif // MAINWINDOW_H
