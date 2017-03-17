#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

class GLViewer;
class DataManager;
class ParameterSet;
class ParameterSetWidget;
class CalculationThread;
class ioThread;

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	void init();
	void CreateActions();
	void CreateMenus();
	void CreateToolBars();
	void CreateStatusBar();

	void CloseWidget();
	void ShowWidget();
	void SetActionStatus(bool value);

	private slots:
	void ImportMesh();
	void ExportMesh();
	void TransToNoisyMesh();
	void TransToOriginalMesh();
	void TransToDenoisedMesh();
	void ClearMesh();
	void ApplyAlgorithms(QString algorithm_name);

	void ShowNoiseWidget();
	void ShowBilateralMeshDenoisingWidget();
	void ShowScanPathPlanWidget();//add for SPP,2.23.2017
	void ShowNonIterativeFeaturePreservingMeshFilteringWidget();
	void ShowFastAndEffectiveFeaturePreservingMeshDenoisingWidget();
	void ShowBilateralNormalFilteringForMeshDenoisingWidget();
	void ShowMeshDenoisingViaL0MinimizationWidget();
	void ShowGuidedMeshNormalFilteringWidget();

	void setActionAndWidget(bool value1, bool value2);
	void needToUpdateGL(bool value);

	void ShowSingleLayer();//add 3.13.2017

	void About();

private:
	Ui::MainWindow *ui;

private:
	QMenu *menu_file_;
	QMenu *menu_algorithms_;
	QMenu *menu_help_;
	QMenu *menu_view_;

	QAction *action_import_mesh_;
	QAction *action_export_mesh_;
	QAction *action_exit_;

	QAction *action_noise_;
	QAction *action_bilateral_mesh_denoising_;
	QAction *action_scan_path_plan_;//add for SPP 2.23.2017
	QAction *action_non_iterative_feature_preserving_mesh_filtering_;
	QAction *action_fast_and_effective_feature_preserving_mesh_denoising_;
	QAction *action_bilateral_normal_filtering_for_mesh_denoising_;
	QAction *action_mesh_denoising_via_l0_minimization_;
	QAction *action_guided_mesh_normal_filtering_;

	QAction *action_show_single_layer_;///add 3.13.2017

	QAction *action_about_;

	QAction *action_render_points_;
	QAction *action_render_edges_;
	QAction *action_render_faces_;
	QAction *action_render_layers_;//add for layers 3-8-2017
	QAction *action_render_hexagonal_subareas_;//add for hexagonal subareas 3-8-2017
	QAction *action_render_intervals_;//add for intervals 3.14.2017
	QAction *action_render_parallel_hatches_;//add for parallel hatches of hexagonal subareas 3.15.2017
	QAction *action_set_background_color_;

	QAction *action_to_noisy_mesh_;
	QAction *action_to_original_mesh_;
	QAction *action_to_denoised_mesh_;
	QAction *action_clear_mesh_;

	QToolBar *toolbar_file_;
	QToolBar *toolbar_opengl_info_;
	QToolBar *toolbar_mesh_status_;

	QLabel *label_operation_info_;

private:
	// glviewer
	GLViewer *opengl_viewer_;
	// datamanager
	DataManager *data_manager_;
	// parameter set
	ParameterSet *parameter_set_;
	// parameter set widget
	ParameterSetWidget *parameter_set_widget_;

	ioThread *io_thread_;
	CalculationThread *calculation_thread_;
};

#endif // MAINWINDOW_H
