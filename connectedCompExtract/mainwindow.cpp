#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_vtkOpenGLWidget = new QVTKOpenGLWidget(this);
    m_vtkOpenGLWidget->setMinimumSize(QSize(800, 600));
    connectedCompExtract();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectedCompExtract()
{

    vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->SetRadius(10);
    sphereSource->SetThetaResolution(10);   //���þ��߷������
    sphereSource->SetPhiResolution(10);    //����γ�߷������
    sphereSource->Update();

    vtkSmartPointer<vtkConeSource> coneSource = vtkSmartPointer<vtkConeSource>::New(); //Բ׶
    coneSource->SetRadius(5);
    coneSource->SetHeight(10);
    coneSource->SetCenter(25, 0, 0);
    coneSource->Update();

    //�ϲ�����
    vtkSmartPointer<vtkAppendPolyData> appendFilter = vtkSmartPointer<vtkAppendPolyData>::New();
    appendFilter->AddInputData(sphereSource->GetOutput());
    appendFilter->AddInputData(coneSource->GetOutput());
    appendFilter->Update();

    //��ͨ�������
    //SetExtractionModeToLargestRegion()��������ȡ��������������ͨ����
    //SetExtractionModeToAllRegions()����ģʽ��Ҫ������ͨ�����ǣ���Ϻ���ColorRegionsOn()ʹ�ã�����ͨ�������ǵ�ͬʱ������һ����ΪRegionId�ĵ��������ݡ�
    //SetExtractionModeToSpecifiedRegions()����ģʽ������ȡһ��������ͨ�����ڸ�ģʽ�£���Ҫͨ��AddSpecifiedRegion()�������Ҫ��ȡ������ţ�����Ŵ��㿪ʼ��
    //SetExtractionModeToClosestPointRegion()����ģʽ��Ҫʹ��SetClosestPoint()��������һ���ռ�����ִ꣬�н��Ϊ��õ��������ͨ����
    //SetExtractionModeToPointSeededRegions()����ģʽ����Ҫʹ��AddSeed()����������ӵ㣬��ȡ���ӵ����ڵ�����
    //SetExtractionModeToCellSeededRegions()����ģʽ����Ҫʹ��AddSeed()����������ӵ�Ԫ����ȡ���ӵ�Ԫ���ڵ�����
    vtkSmartPointer<vtkPolyDataConnectivityFilter> connectivityFilter = vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
    connectivityFilter->SetInputData(appendFilter->GetOutput());
    connectivityFilter->SetExtractionModeToCellSeededRegions();
    connectivityFilter->AddSeed(100);
    connectivityFilter->Update();

    vtkSmartPointer<vtkPolyDataMapper> originalMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    originalMapper->SetInputConnection(appendFilter->GetOutputPort());
    originalMapper->Update();
    vtkSmartPointer<vtkActor> originalActor = vtkSmartPointer<vtkActor>::New();
    originalActor->SetMapper(originalMapper);

    vtkSmartPointer<vtkPolyDataMapper> extractedMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    extractedMapper->SetInputConnection(connectivityFilter->GetOutputPort());
    extractedMapper->Update();
    vtkSmartPointer<vtkActor> extractedActor = vtkSmartPointer<vtkActor>::New();
    extractedActor->SetMapper(extractedMapper);

    double leftViewport[4] = { 0.0, 0.0, 0.5, 1.0 };
    double rightViewport[4] = { 0.5, 0.0, 1.0, 1.0 };

    vtkSmartPointer<vtkRenderer> leftRenderer = vtkSmartPointer<vtkRenderer>::New();
    leftRenderer->SetViewport(leftViewport);
    leftRenderer->AddActor(originalActor);
    leftRenderer->SetBackground(1, 0, 0);

    vtkSmartPointer<vtkRenderer> rightRenderer = vtkSmartPointer<vtkRenderer>::New();
    rightRenderer->SetViewport(rightViewport);
    rightRenderer->AddActor(extractedActor);
    rightRenderer->SetBackground(0, 0, 0);

    m_vtkOpenGLWidget->renderWindow()->AddRenderer(leftRenderer);
    m_vtkOpenGLWidget->renderWindow()->AddRenderer(rightRenderer);
    m_vtkOpenGLWidget->renderWindow()->Render();

}

