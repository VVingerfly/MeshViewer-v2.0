//===============================================================
// Author       : Wei Li (lw15@mail.ustc.edu.cn)
// Created Date : 2017/Jan/10 13:33:03
// Last Change  : 2017/Jan/10 13:33:05
// Summary      :
//===============================================================

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include "MeshViewer.h"

class MeshViewer;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	void CreateActionGroup();
	void CreateAction();
	void CreateActionView();
	void CreateActionSelection();
	void CreateStatusBar();


public slots:
	void setRenderMode(MeshRenderMode mode);
	void showMeshInfo(int npoint, int nedge, int nface);
private:
	Ui::MainWindowClass			ui;
	MeshViewer				   *meshViewer_;

	QLabel* label_meshinfo_;
	QLabel *label_operatorinfo_;
};

#endif // MAINWINDOW_H
