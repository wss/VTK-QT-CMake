#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_vtkOpenGlwidget = new QVTKOpenGLWidget(this);
    m_bmpReader = vtkSmartPointer<vtkBMPReader>::New();
    m_vtkImageViewer2 = vtkSmartPointer<vtkImageViewer2>::New();
    // ����RendererWindows �Ͳ��ᵯ����������
    m_vtkImageViewer2->SetRenderWindow(m_vtkOpenGlwidget->renderWindow());
    changeImagePixel();
    m_vtkOpenGlwidget->setMinimumSize(QSize(800,600));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeImagePixel()
{
    m_bmpReader->SetFileName("lena.bmp");
    m_bmpReader->Update();
    int subRegion[6] = { 0,300, 0, 300, 0, 0 };  //��ȡ0-300�����ε�����
    vtkImageIterator<unsigned char> it(m_bmpReader->GetOutput(), subRegion);
    while (!it.IsAtEnd())
    {
        unsigned char* inSI = it.BeginSpan();
        unsigned char* inSIEnd = it.EndSpan();
        while (inSI != inSIEnd)
        {
            *inSI = 255 - *inSI;   //����ֵȡ��
            ++inSI;
        }
        it.NextSpan();
    }
    // ������ 
    m_vtkImageViewer2->SetInputConnection(m_bmpReader->GetOutputPort());
    // ������Ⱦ
    m_vtkImageViewer2->Render();
}

