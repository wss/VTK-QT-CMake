#include "mainwindow.h"
#include "./ui_mainwindow.h"


#include <QDebug>

#include <vtkSmartPointer.h>
//���ɴ��ն�������
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkIdTypeArray.h>
#include <vtkSelectionNode.h>
#include <vtkSelection.h>
#include <vtkExtractSelection.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkInformation.h>
#include <vtkProperty.h>
//����Լ��
#include <vtkPolyData.h>
#include <vtkFeatureEdges.h>
#include <vtkPolyDataMapper.h>
#include <vtkFillHolesFilter.h>
#include <vtkPolyDataNormals.h>
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

#include <vtkRenderWindowInteractor.h>
#include <vtkAutoInit.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    closedDetect();
    m_vtkOpenGLWidget = new QVTKOpenGLWidget(this);
    m_vtkOpenGLWidget->setMinimumSize(QSize(800,600));
    m_vtkOpenGLWidget->renderWindow()->AddRenderer(m_leftRenderer);
    m_vtkOpenGLWidget->renderWindow()->AddRenderer(m_rightRenderer);
    m_vtkOpenGLWidget->renderWindow()->Render();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closedDetect()
{
    //���ɴ��ն�С��
    vtkSmartPointer<vtkPolyData> input = vtkSmartPointer<vtkPolyData>::New();
    generateData(input);

    vtkSmartPointer<vtkFeatureEdges> featureEdges = vtkSmartPointer<vtkFeatureEdges>::New();
    featureEdges->SetInputData(input);
    featureEdges->BoundaryEdgesOn();  //�򿪱߽���ȡ
    featureEdges->FeatureEdgesOff();  //�ر���ȡ������
    featureEdges->ManifoldEdgesOff(); //�ر����б�:ֻ����������ΰ����ı�
    featureEdges->NonManifoldEdgesOff();  //�رշ����б�:�����������ϸ�����ΰ����ı�
    featureEdges->Update();

    vtkIdType numberOfOpenEdges = featureEdges->GetOutput()->GetNumberOfCells(); //��ü��ıߵĸ���
    if (numberOfOpenEdges)
    {
        qDebug() << "������ģ�Ͳ��Ƿ�յ�...";
    }
    else
    {
        qDebug() << "������ģ���Ƿ�յ�...";
    }
    vtkSmartPointer<vtkFillHolesFilter> fillHolesFilter = vtkSmartPointer<vtkFillHolesFilter>::New();
    fillHolesFilter->SetInputData(input);
    fillHolesFilter->Update();

    //�����������������ھ���©����䣬ģ�͵����е�Ԫ�ĵ�˳�򲢲�һ�£����ʹ��vtkPolyDataNormals::ConsisitencyOn()���е�����
    vtkSmartPointer<vtkPolyDataNormals> normals = vtkSmartPointer<vtkPolyDataNormals>::New();
    normals->SetInputConnection(fillHolesFilter->GetOutputPort());
    normals->ConsistencyOn();   //����Ҫ������������Ԫ���˳�����������˳��
    normals->SplittingOff();  //������Ե
    normals->Update();

    double leftViewport[4] = { 0.0, 0.0, 0.5, 1.0 };
    double rightViewport[4] = { 0.5, 0.0, 1.0, 1.0 };

    //ԭ����mapper
    vtkSmartPointer<vtkPolyDataMapper> originalMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    originalMapper->SetInputData(input);
    vtkSmartPointer<vtkProperty> backfaceProp = vtkSmartPointer<vtkProperty>::New();
    backfaceProp->SetDiffuseColor(0.89, 0.81, 0.34);  //actor������ɫ��������ʾ�ն�
    vtkSmartPointer<vtkActor> originalActor = vtkSmartPointer<vtkActor>::New();
    originalActor->SetMapper(originalMapper);
    originalActor->SetBackfaceProperty(backfaceProp);
    originalActor->GetProperty()->SetDiffuseColor(1.0, 0.3882, 0.2784);  //actor��ɫ

    //�߽�mapper
    vtkSmartPointer<vtkPolyDataMapper> edgeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    edgeMapper->SetInputData(featureEdges->GetOutput());
    vtkSmartPointer<vtkActor> edgeActor = vtkSmartPointer<vtkActor>::New();
    edgeActor->SetMapper(edgeMapper);
    edgeActor->GetProperty()->SetEdgeColor(0.0, 0.0, 1.0);  //�߽���ɫ
    edgeActor->GetProperty()->SetEdgeVisibility(1);
    edgeActor->GetProperty()->SetLineWidth(5);

    //���׺��mapper
    vtkSmartPointer<vtkPolyDataMapper> filledMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    filledMapper->SetInputData(normals->GetOutput());
    vtkSmartPointer<vtkActor> filledActor = vtkSmartPointer<vtkActor>::New();
    filledActor->SetMapper(filledMapper);
    filledActor->GetProperty()->SetDiffuseColor(1.0, 0.3882, 0.2784);

    m_leftRenderer = vtkSmartPointer<vtkRenderer>::New();
    m_leftRenderer->SetViewport(leftViewport);
    m_leftRenderer->AddActor(originalActor);
    m_leftRenderer->AddActor(edgeActor);
    m_leftRenderer->SetBackground(1.0, 1.0, 1.0);

    m_rightRenderer = vtkSmartPointer<vtkRenderer>::New();
    m_rightRenderer->SetViewport(rightViewport);
    m_rightRenderer->AddActor(filledActor);
    m_rightRenderer->SetBackground(0, 0, 0);

}

void MainWindow::generateData(vtkSmartPointer<vtkPolyData> input)
{
    // ����һ������
    vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->Update();

    //�ṩ�˲���ͼ���ֵ�ķ����������Զ�������С�Ա���������
    vtkSmartPointer<vtkIdTypeArray> ids = vtkSmartPointer<vtkIdTypeArray>::New();
    ids->SetNumberOfComponents(1);
    ids->InsertNextValue(2);
    ids->InsertNextValue(10);

    //ѡ�����еĽ�,���ڴ洢ѡ����
    vtkSmartPointer<vtkSelectionNode> selectionNode = vtkSmartPointer<vtkSelectionNode>::New();
    selectionNode->SetFieldType(vtkSelectionNode::CELL);
    selectionNode->SetContentType(vtkSelectionNode::INDICES);
    selectionNode->SetSelectionList(ids);
    selectionNode->GetProperties()->Set(vtkSelectionNode::INVERSE(), 1);

    vtkSmartPointer<vtkSelection> selection = vtkSmartPointer<vtkSelection>::New();
    selection->AddNode(selectionNode);

    //��vtkdataset��ȡ�Ӽ�,ɾ������
    vtkSmartPointer<vtkExtractSelection> extractSelection = vtkSmartPointer<vtkExtractSelection>::New();
    extractSelection->SetInputData(0, sphereSource->GetOutput());
    extractSelection->SetInputData(1, selection);
    extractSelection->Update();

    //vtkDataSetSurfaceFilter�Ǹ���汾��vtkgeometry�˲���
    //����û��һ��ѡ��Χ����vtkGeometryFilterʹ�ø�����ڴ�
    //ֻ��һ��ѡ������ṹ����ʱ�Ƿ�ʹ����������
    vtkSmartPointer<vtkDataSetSurfaceFilter> surfaceFilter = vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
    surfaceFilter->SetInputConnection(extractSelection->GetOutputPort());
    surfaceFilter->Update();

    input->ShallowCopy(surfaceFilter->GetOutput());
}

