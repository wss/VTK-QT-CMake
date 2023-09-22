#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    color2gray();
    m_vtkOpenGLWidget = new QVTKOpenGLWidget(this);
    m_vtkOpenGLWidget->setMinimumSize(QSize(800,600));
    m_vtkOpenGLWidget->renderWindow()->AddRenderer(m_originalRenderer);
    m_vtkOpenGLWidget->renderWindow()->AddRenderer(m_shiftscaleRenderer);
    m_vtkOpenGLWidget->renderWindow()->Render();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::color2gray()
{
    vtkSmartPointer<vtkBMPReader> reader = vtkSmartPointer<vtkBMPReader>::New();
    reader->SetFileName("lena.bmp");

    //luminance = 0.3*R + 0.59*G + 0.11*B
    vtkSmartPointer<vtkImageLuminance> luminanceFilter = vtkSmartPointer<vtkImageLuminance>::New();
    luminanceFilter->SetInputConnection(reader->GetOutputPort());   //用SetInputData不行
    luminanceFilter->Update();

    vtkSmartPointer<vtkImageActor> originalActor = vtkSmartPointer<vtkImageActor>::New();
    originalActor->GetMapper()->SetInputConnection(reader->GetOutputPort());  //用SetInputData不行

    vtkSmartPointer<vtkImageActor> shiftscaleActor = vtkSmartPointer<vtkImageActor>::New();
    shiftscaleActor->GetMapper()->SetInputConnection(luminanceFilter->GetOutputPort());  //用SetInputData不行

    double originalViewport[4] = { 0.0, 0.0, 0.5, 1.0 };
    double shiftscaleViewport[4] = { 0.5, 0.0, 1.0, 1.0 };

    m_originalRenderer = vtkSmartPointer<vtkRenderer>::New();
    m_originalRenderer->SetViewport(originalViewport);
    m_originalRenderer->AddActor(originalActor);
    m_originalRenderer->SetBackground(1.0, 1.0, 1.0);
    m_originalRenderer->ResetCamera();

    m_shiftscaleRenderer = vtkSmartPointer<vtkRenderer>::New();
    m_shiftscaleRenderer->SetViewport(shiftscaleViewport);
    m_shiftscaleRenderer->AddActor(shiftscaleActor);
    m_shiftscaleRenderer->SetBackground(1.0, 1.0, 1.0);
    m_shiftscaleRenderer->ResetCamera();


}
 
