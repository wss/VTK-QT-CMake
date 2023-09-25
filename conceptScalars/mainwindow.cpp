#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    conceptScalars();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::conceptScalars()
{
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    points->InsertNextPoint(0, 0, 0);
    points->InsertNextPoint(1, 0, 0);

    //�������������
    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
    polyData->SetPoints(points);

    //׼������������ı���ֵ,�ֱ�Ϊ1��2
    vtkSmartPointer<vtkDoubleArray> weights = vtkSmartPointer<vtkDoubleArray>::New();
    weights->SetNumberOfValues(2);
    weights->SetValue(0, 1);
    weights->SetValue(1, 2);
    //�Ȼ�ȡ��������ݵĵ�����ָ�룬Ȼ�����øõ����ݵı�������ֵ��
    polyData->GetPointData()->SetScalars(weights);

    //���������Ϊ0�ĵ�ı���ֵ��
    double weight = vtkDoubleArray::SafeDownCast(polyData->GetPointData()->GetScalars())->GetValue(0);
    qDebug() << "double weight: " << weight;
}

