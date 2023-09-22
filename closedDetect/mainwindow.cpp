#include "mainwindow.h"
#include "./ui_mainwindow.h"


#include <QDebug>

#include <vtkSmartPointer.h>
//生成带空洞网格球
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkIdTypeArray.h>
#include <vtkSelectionNode.h>
#include <vtkSelection.h>
#include <vtkExtractSelection.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkInformation.h>
#include <vtkProperty.h>
//封闭性检测
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
    //生成带空洞小球
    vtkSmartPointer<vtkPolyData> input = vtkSmartPointer<vtkPolyData>::New();
    generateData(input);

    vtkSmartPointer<vtkFeatureEdges> featureEdges = vtkSmartPointer<vtkFeatureEdges>::New();
    featureEdges->SetInputData(input);
    featureEdges->BoundaryEdgesOn();  //打开边界提取
    featureEdges->FeatureEdgesOff();  //关闭提取特征边
    featureEdges->ManifoldEdgesOff(); //关闭流行边:只被两个多边形包含的边
    featureEdges->NonManifoldEdgesOff();  //关闭非流行边:被三个及以上个多边形包含的边
    featureEdges->Update();

    vtkIdType numberOfOpenEdges = featureEdges->GetOutput()->GetNumberOfCells(); //获得检测的边的个数
    if (numberOfOpenEdges)
    {
        qDebug() << "该网格模型不是封闭的...";
    }
    else
    {
        qDebug() << "该网格模型是封闭的...";
    }
    vtkSmartPointer<vtkFillHolesFilter> fillHolesFilter = vtkSmartPointer<vtkFillHolesFilter>::New();
    fillHolesFilter->SetInputData(input);
    fillHolesFilter->Update();

    //建立法向量来，由于经过漏洞填充，模型的所有单元的点顺序并不一致，因此使用vtkPolyDataNormals::ConsisitencyOn()进行调整。
    vtkSmartPointer<vtkPolyDataNormals> normals = vtkSmartPointer<vtkPolyDataNormals>::New();
    normals->SetInputConnection(fillHolesFilter->GetOutputPort());
    normals->ConsistencyOn();   //很重要，根据其他单元点的顺序调整补充点的顺序
    normals->SplittingOff();  //锐利边缘
    normals->Update();

    double leftViewport[4] = { 0.0, 0.0, 0.5, 1.0 };
    double rightViewport[4] = { 0.5, 0.0, 1.0, 1.0 };

    //原来的mapper
    vtkSmartPointer<vtkPolyDataMapper> originalMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    originalMapper->SetInputData(input);
    vtkSmartPointer<vtkProperty> backfaceProp = vtkSmartPointer<vtkProperty>::New();
    backfaceProp->SetDiffuseColor(0.89, 0.81, 0.34);  //actor背面颜色，用来显示空洞
    vtkSmartPointer<vtkActor> originalActor = vtkSmartPointer<vtkActor>::New();
    originalActor->SetMapper(originalMapper);
    originalActor->SetBackfaceProperty(backfaceProp);
    originalActor->GetProperty()->SetDiffuseColor(1.0, 0.3882, 0.2784);  //actor颜色

    //边界mapper
    vtkSmartPointer<vtkPolyDataMapper> edgeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    edgeMapper->SetInputData(featureEdges->GetOutput());
    vtkSmartPointer<vtkActor> edgeActor = vtkSmartPointer<vtkActor>::New();
    edgeActor->SetMapper(edgeMapper);
    edgeActor->GetProperty()->SetEdgeColor(0.0, 0.0, 1.0);  //边界颜色
    edgeActor->GetProperty()->SetEdgeVisibility(1);
    edgeActor->GetProperty()->SetLineWidth(5);

    //补孔后的mapper
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
    // 创建一个球体
    vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->Update();

    //提供了插入和检索值的方法，并会自动调整大小以保存新数据
    vtkSmartPointer<vtkIdTypeArray> ids = vtkSmartPointer<vtkIdTypeArray>::New();
    ids->SetNumberOfComponents(1);
    ids->InsertNextValue(2);
    ids->InsertNextValue(10);

    //选择树中的节,用于存储选择结果
    vtkSmartPointer<vtkSelectionNode> selectionNode = vtkSmartPointer<vtkSelectionNode>::New();
    selectionNode->SetFieldType(vtkSelectionNode::CELL);
    selectionNode->SetContentType(vtkSelectionNode::INDICES);
    selectionNode->SetSelectionList(ids);
    selectionNode->GetProperties()->Set(vtkSelectionNode::INVERSE(), 1);

    vtkSmartPointer<vtkSelection> selection = vtkSmartPointer<vtkSelection>::New();
    selection->AddNode(selectionNode);

    //从vtkdataset提取子集,删除操作
    vtkSmartPointer<vtkExtractSelection> extractSelection = vtkSmartPointer<vtkExtractSelection>::New();
    extractSelection->SetInputData(0, sphereSource->GetOutput());
    extractSelection->SetInputData(1, selection);
    extractSelection->Update();

    //vtkDataSetSurfaceFilter是更快版本的vtkgeometry滤波器
    //但它没有一个选择范围。比vtkGeometryFilter使用更多的内存
    //只有一个选择：输入结构类型时是否使用三角形条
    vtkSmartPointer<vtkDataSetSurfaceFilter> surfaceFilter = vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
    surfaceFilter->SetInputConnection(extractSelection->GetOutputPort());
    surfaceFilter->Update();

    input->ShallowCopy(surfaceFilter->GetOutput());
}

