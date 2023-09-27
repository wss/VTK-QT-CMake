#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <qdebug.h>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());
    ui->setupUi(this);
    m_nativeWidget = new QVTKOpenGLNativeWidget(this);
    m_nativeWidget->setGeometry(0, 0, 800, 600);
    cylinder();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::cylinder() {
    //source
    vtkSmartPointer<vtkCylinderSource> cylinder = vtkSmartPointer<vtkCylinderSource>::New();
    cylinder->SetHeight(3.0);
    cylinder->SetRadius(1.0);
    cylinder->SetResolution(20);   //Բ�������������ָ�������ɣ��籾����20������ɵĽ���Բ��

    //ӳ��
    vtkSmartPointer<vtkPolyDataMapper> cylinderMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    cylinderMapper->SetInputConnection(cylinder->GetOutputPort());

    //actor
    vtkSmartPointer<vtkActor> cylinderActor = vtkSmartPointer<vtkActor>::New();
    cylinderActor->SetMapper(cylinderMapper);
    //cylinderActor->GetProperty()->SetColor(1.0, 0.0, 0.0); //��Բ��Ϊ��ɫ

    //render
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(cylinderActor);
    renderer->SetBackground(0.0, 0.0, 0.0); //����Ϊ��ɫ

    m_nativeWidget->renderWindow()->AddRenderer(renderer);
    m_nativeWidget->renderWindow()->Render();

    

    //���Ӳ�ͬ�ƹ�ʹԲ����չ�ֲ�ͬ��ɫ
    vtkSmartPointer<vtkLight> light1 = vtkSmartPointer<vtkLight>::New();
    light1->SetColor(0, 1, 0);
    light1->SetPosition(0, 0, 1);
    light1->SetFocalPoint(renderer->GetActiveCamera()->GetFocalPoint());
    renderer->AddLight(light1);

    vtkSmartPointer<vtkLight> light2 = vtkSmartPointer<vtkLight>::New();
    light2->SetColor(0, 0, 1);
    light2->SetPosition(0, 0, -1);
    light2->SetFocalPoint(renderer->GetActiveCamera()->GetFocalPoint());
    renderer->AddLight(light2);

    vtkSmartPointer<vtkLight> light3 = vtkSmartPointer<vtkLight>::New();
    light3->SetColor(1, 0, 0);
    light3->SetPosition(0, 1, 0);
    light3->SetFocalPoint(renderer->GetActiveCamera()->GetFocalPoint());
    renderer->AddLight(light3);

    vtkSmartPointer<vtkLight> light4 = vtkSmartPointer<vtkLight>::New();
    light4->SetColor(1, 0, 0);
    light4->SetPosition(0, -1, 0);
    light4->SetFocalPoint(renderer->GetActiveCamera()->GetFocalPoint());
    renderer->AddLight(light4);

}
