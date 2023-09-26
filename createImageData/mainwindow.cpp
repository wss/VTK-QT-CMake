#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_vtkOpenGLWidget = new QVTKOpenGLWidget(this);
    m_vtkOpenGLWidget->setGeometry(0, 0, 800, 600);
    createImageData();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createImageData()
{
    vtkSmartPointer<vtkImageData> imageData = vtkSmartPointer<vtkImageData>::New();
    vtkSmartPointer<vtkInformation> info = vtkSmartPointer<vtkInformation>::New();
    imageData->SetDimensions(16, 16, 1);  //ָ��ά��
    imageData->SetScalarType(VTK_UNSIGNED_CHAR, info);   //ͼ����������
    imageData->SetNumberOfScalarComponents(1, info);   //ÿ��������Ҫ��ʾ����� =1��ָ����ͼ
    imageData->AllocateScalars(info);  //�����ڴ�����ͼ�����ݣ�Ĭ����������ֵΪ0
    unsigned char* ptr = (unsigned char*)imageData->GetScalarPointer();
    for (int i = 0; i < 16 * 16 * 1; i++) {
        *ptr = i % 256;
        ptr++;
    }
    //actor
    vtkSmartPointer<vtkImageActor> actor = vtkSmartPointer<vtkImageActor>::New();
    actor->SetInputData(imageData);
    double viewport[4] = { 0,0,1,1 };
    //render
    vtkSmartPointer<vtkRenderer> render = vtkSmartPointer<vtkRenderer>::New();
    render->AddActor(actor);
    render->SetViewport(viewport);  //�ĸ���Ϊmin x, min y, max x, max y,ȡ֮��ΧΪ0-1
    render->ResetCamera();
    render->SetBackground(1, 1, 1);
    m_vtkOpenGLWidget->renderWindow()->AddRenderer(render);
    m_vtkOpenGLWidget->renderWindow()->Render();

}

