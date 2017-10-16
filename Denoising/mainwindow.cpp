#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "glviewer.h"
#include "datamanager.h"
#include "parameterset.h"
#include "parametersetwidget.h"
#include "calculationthread.h"
#include "iothread.h"

MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	setWindowTitle("MeshUI");
	setWindowIcon(QIcon(":Icons/MeshDenoise.ico"));

	setGeometry(250, 50, 880, 660);

	init();
	CreateActions();
	CreateMenus();
	CreateToolBars();
	CreateStatusBar();

	QVBoxLayout *layout_left_ = new QVBoxLayout;
	layout_left_->addStretch(2);

	QHBoxLayout *layout_main = new QHBoxLayout;
	layout_main->addLayout(layout_left_);
	layout_main->addWidget(opengl_viewer_);
	layout_main->setStretch(1, 1);
	this->centralWidget()->setLayout(layout_main);
}

MainWindow::~MainWindow()
{
	CloseWidget();

	if (calculation_thread_ != NULL)
		delete calculation_thread_;
	calculation_thread_ = NULL;

	if (opengl_viewer_ != NULL)
		delete opengl_viewer_;
	opengl_viewer_ = NULL;

	if (data_manager_ != NULL)
		delete data_manager_;
	data_manager_ = NULL;

	if (parameter_set_ != NULL)
		delete parameter_set_;
	parameter_set_ = NULL;
}

void MainWindow::init()
{
	opengl_viewer_ = new GLViewer(this);
	data_manager_ = new DataManager();
	parameter_set_ = new ParameterSet();
	parameter_set_widget_ = NULL;
	io_thread_ = new ioThread(data_manager_);
	io_thread_->io_type_ = ioThread::kNon;
	connect(io_thread_, SIGNAL(needToUpdateGL(bool)), this, SLOT(needToUpdateGL(bool)));
	connect(io_thread_, SIGNAL(setActionAndWidget(bool, bool)), this, SLOT(setActionAndWidget(bool, bool)));
	calculation_thread_ = new CalculationThread();
	calculation_thread_->algorithms_type_ = CalculationThread::kNon;
	connect(calculation_thread_, SIGNAL(needToUpdateGL(bool)), this, SLOT(needToUpdateGL(bool)));
	connect(calculation_thread_, SIGNAL(setActionAndWidget(bool, bool)), this, SLOT(setActionAndWidget(bool, bool)));
}

void MainWindow::CreateActions()
{
	action_import_mesh_ = new QAction(QIcon(":/Icons/open.ico"), tr("Import Mesh"), this);
	action_import_mesh_->setStatusTip("Import Mesh.");
	connect(action_import_mesh_, SIGNAL(triggered()), this, SLOT(ImportMesh()));

	action_export_mesh_ = new QAction(QIcon(":/Icons/save.ico"), tr("Export Mesh"), this);
	action_export_mesh_->setStatusTip("Export Mesh.");
	action_export_mesh_->setEnabled(false);
	connect(action_export_mesh_, SIGNAL(triggered()), this, SLOT(ExportMesh()));

	action_exit_ = new QAction(tr("Exit"), this);
	action_exit_->setStatusTip("Exit.");
	connect(action_exit_, SIGNAL(triggered()), this, SLOT(close()));

	action_render_points_ = new QAction(QIcon(":/Icons/points.ico"), tr("Render Points"), this);
	action_render_points_->setStatusTip("Render Points.");
	action_render_points_->setCheckable(true);
	connect(action_render_points_, SIGNAL(toggled(bool)), opengl_viewer_, SLOT(setDrawPointsStatus(bool)));

	action_render_edges_ = new QAction(QIcon(":/Icons/edges.ico"), tr("Render Edges"), this);
	action_render_edges_->setStatusTip("Render Edges.");
	action_render_edges_->setCheckable(true);
	connect(action_render_edges_, SIGNAL(toggled(bool)), opengl_viewer_, SLOT(setDrawEdgesStatus(bool)));

	action_render_faces_ = new QAction(QIcon(":/Icons/faces.ico"), tr("Render Faces"), this);
	action_render_faces_->setStatusTip("Render Faces.");
	action_render_faces_->setCheckable(true);
	action_render_faces_->setChecked(true);
	connect(action_render_faces_, SIGNAL(toggled(bool)), opengl_viewer_, SLOT(setDrawFacesStatus(bool)));

	action_render_layers_ = new QAction(QIcon("layers.ico"), tr("Render Layers"), this);//add for slice layers 3-8-2017
	action_render_layers_->setStatusTip("Render Layers.");
	action_render_layers_->setCheckable(true);
	//action_render_layers_->setChecked(true);
	connect(action_render_layers_, SIGNAL(toggled(bool)), opengl_viewer_, SLOT(setDrawLayersStatus(bool)));

	action_render_staircase_ = new QAction(QIcon("staircase.ico"), tr("Render Staircase"), this);//add for staircase 8-31-2017
	action_render_staircase_->setStatusTip("Render Stairacase.");
	action_render_staircase_->setCheckable(true);
	connect(action_render_staircase_, SIGNAL(toggled(bool)), opengl_viewer_, SLOT(setDrawStaircase(bool)));

	action_render_hexagonal_subareas_ = new QAction(QIcon("hexagon.ico"), tr("Render Hexagonal Subareas"), this);//add for hexagonal subareas 3-9-2017
	action_render_hexagonal_subareas_->setStatusTip("Render Hexagonal Subareas.");
	action_render_hexagonal_subareas_->setCheckable(true);
	connect(action_render_hexagonal_subareas_, SIGNAL(toggled(bool)), opengl_viewer_, SLOT(setDrawHexagonsStatus(bool)));

	action_render_intervals_ = new QAction(QIcon("interval.ico"), tr("Render intervals"), this);//add for intervals 3.14.2017
	action_render_intervals_->setStatusTip("Render Intervals.");
	action_render_intervals_->setCheckable(true);
	connect(action_render_intervals_, SIGNAL(toggled(bool)), opengl_viewer_, SLOT(setDrawIntervalsStatus(bool)));

	action_render_hexagonal_subareas_parallel_hatches_ = new QAction(QIcon("ParallelHatches.ico"), tr("Render Hexagonal Subareas Hatches"), this);//add for hexagonal subareas parallel hatches 3.15.2017
	action_render_hexagonal_subareas_parallel_hatches_->setStatusTip("Render Parallel hatches.");
	action_render_hexagonal_subareas_parallel_hatches_->setCheckable(true);
	connect(action_render_hexagonal_subareas_parallel_hatches_, SIGNAL(toggled(bool)), opengl_viewer_, SLOT(setDrawParallelHatchessStatus(bool)));

	action_render_interval_hatches_ = new QAction(QIcon("intervalHatches.ico"), tr("Render Interval Hatches"), this);//add for interval hatches 3.21.2017
	action_render_interval_hatches_->setStatusTip("Render Interval hatches.");
	action_render_interval_hatches_->setCheckable(true);
	connect(action_render_interval_hatches_, SIGNAL(toggled(bool)), opengl_viewer_, SLOT(setDrawIntervalTrianglesHatchessStatus(bool)));

	action_set_background_color_ = new QAction(QIcon(":/Icons/background.ico"), tr("Change Background Color"), this);
	action_set_background_color_->setStatusTip("Change Background Color.");
	connect(action_set_background_color_, SIGNAL(triggered()), opengl_viewer_, SLOT(setBackgroundColor()));

	action_to_original_mesh_ = new QAction(this);
	action_to_original_mesh_->setText("Original Mesh");
	action_to_original_mesh_->setStatusTip("Show Original Mesh.");
	connect(action_to_original_mesh_, SIGNAL(triggered()), this, SLOT(TransToOriginalMesh()));

	action_to_noisy_mesh_ = new QAction(this);
	action_to_noisy_mesh_->setText("Noisy Mesh");
	action_to_noisy_mesh_->setStatusTip("Show Noisy Mesh.");
	connect(action_to_noisy_mesh_, SIGNAL(triggered()), this, SLOT(TransToNoisyMesh()));

	action_to_denoised_mesh_ = new QAction(this);
	action_to_denoised_mesh_->setText("Denoised Mesh");
	action_to_denoised_mesh_->setStatusTip("Show Denoised Mesh.");
	connect(action_to_denoised_mesh_, SIGNAL(triggered()), this, SLOT(TransToDenoisedMesh()));

	action_clear_mesh_ = new QAction(this);
	action_clear_mesh_->setText("Clear");
	action_clear_mesh_->setStatusTip("Clear Mesh.");
	connect(action_clear_mesh_, SIGNAL(triggered()), this, SLOT(ClearMesh()));

	action_noise_ = new QAction(tr("Noise"), this);
	action_noise_->setStatusTip("Add Noise to Mesh using Algorithm -- Noise.");
	connect(action_noise_, SIGNAL(triggered()), this, SLOT(ShowNoiseWidget()));

	action_bilateral_mesh_denoising_ = new QAction(tr("Bilateral Mesh Denoising"), this);
	action_bilateral_mesh_denoising_->setStatusTip("Denoise Mesh using Algorithm -- Bilateral Mesh Denosing.");
	connect(action_bilateral_mesh_denoising_, SIGNAL(triggered()), this, SLOT(ShowBilateralMeshDenoisingWidget()));

	action_scan_path_plan_ = new QAction(tr("Scan Path Plan"), this);
	action_scan_path_plan_->setStatusTip("Spp -- Scan Path Plan.");
	connect(action_scan_path_plan_, SIGNAL(triggered()), this, SLOT(ShowScanPathPlanWidget()));////add for SPP,2.23.2017

	action_simple_spp_= new QAction(tr("SimpleSpp"), this);
	action_simple_spp_->setStatusTip("SimpleSpp --Simple Scan Path Plan.");
	connect(action_simple_spp_, SIGNAL(triggered()), this, SLOT(ShowSimpleSPPWidget()));////add for SimpleSPP,10.2.2017

	action_non_iterative_feature_preserving_mesh_filtering_ = new QAction(tr("Non-Iterative, Feature Preserving Mesh Filtering"), this);
	action_non_iterative_feature_preserving_mesh_filtering_->setStatusTip("Denoise Mesh using Algorithm -- Non-Iterative, Feature Preserving Mesh Filtering.");
	connect(action_non_iterative_feature_preserving_mesh_filtering_, SIGNAL(triggered()), this, SLOT(ShowNonIterativeFeaturePreservingMeshFilteringWidget()));

	action_fast_and_effective_feature_preserving_mesh_denoising_ = new QAction(tr("Fast and Effective Feature-Preserving Mesh Denoising"), this);
	action_fast_and_effective_feature_preserving_mesh_denoising_->setStatusTip("Denoise Mesh using Algorithm -- Fast and Effective Feature-Preserving Mesh Denoising.");
	connect(action_fast_and_effective_feature_preserving_mesh_denoising_, SIGNAL(triggered()), this, SLOT(ShowFastAndEffectiveFeaturePreservingMeshDenoisingWidget()));

	action_bilateral_normal_filtering_for_mesh_denoising_ = new QAction(tr("Bilateral Normal Filtering for Mesh Denoising"), this);
	action_bilateral_normal_filtering_for_mesh_denoising_->setStatusTip("Denoise Mesh using Algorithm -- Bilateral Normal Filtering for Mesh Denoising.");
	connect(action_bilateral_normal_filtering_for_mesh_denoising_, SIGNAL(triggered()), this, SLOT(ShowBilateralNormalFilteringForMeshDenoisingWidget()));

	action_mesh_denoising_via_l0_minimization_ = new QAction(tr("Mesh Denoising via L0 Minimization"), this);
	action_mesh_denoising_via_l0_minimization_->setStatusTip("Denoise Mesh using Algorithm -- Mesh Denoising via L0 Minimization.");
	connect(action_mesh_denoising_via_l0_minimization_, SIGNAL(triggered()), this, SLOT(ShowMeshDenoisingViaL0MinimizationWidget()));

	action_guided_mesh_normal_filtering_ = new QAction(tr("Guided Mesh Normal Filtering"), this);
	action_guided_mesh_normal_filtering_->setStatusTip("Denoise Mesh using Algorithm -- Guided Mesh Normal Filtering.");
	connect(action_guided_mesh_normal_filtering_, SIGNAL(triggered()), this, SLOT(ShowGuidedMeshNormalFilteringWidget()));

	action_show_single_layer_ = new QAction(tr("Show single layer"), this);//add 3.13.2017
	action_show_single_layer_->setStatusTip("SPP -- Show a specific layer of the sliced model");
	connect(action_show_single_layer_, SIGNAL(triggered()), this, SLOT(ShowSingleLayer()));

	action_about_ = new QAction(QIcon(":/Icons/about.ico"), tr("About"), this);
	action_about_->setStatusTip("Information about this UI.");
	connect(action_about_, SIGNAL(triggered()), this, SLOT(About()));
}

void MainWindow::CreateMenus()
{
	menu_file_ = menuBar()->addMenu(tr("File"));
	menu_file_->addAction(action_import_mesh_);
	menu_file_->addAction(action_export_mesh_);
	menu_file_->addSeparator();
	menu_file_->addAction(action_exit_);

	menu_algorithms_ = menuBar()->addMenu(tr("Algorithms"));
	menu_algorithms_->addAction(action_noise_);
	menu_algorithms_->addSeparator();
	menu_algorithms_->addAction(action_bilateral_mesh_denoising_);	
	menu_algorithms_->addAction(action_non_iterative_feature_preserving_mesh_filtering_);
	menu_algorithms_->addAction(action_fast_and_effective_feature_preserving_mesh_denoising_);
	menu_algorithms_->addAction(action_bilateral_normal_filtering_for_mesh_denoising_);
	menu_algorithms_->addAction(action_mesh_denoising_via_l0_minimization_);
	menu_algorithms_->addAction(action_guided_mesh_normal_filtering_);
	menu_algorithms_->addSeparator();
	menu_algorithms_->addAction(action_scan_path_plan_);//add for SPP 2.23.2017
	menu_algorithms_->addAction(action_simple_spp_);//add for SimpleSPP 10.2.2017
	menu_algorithms_->setEnabled(false);

	menu_view_ = menuBar()->addMenu(tr("View"));//add 3.13.2017
	menu_view_->addAction(action_show_single_layer_);
	menu_view_->setEnabled(false);

	menu_help_ = menuBar()->addMenu(tr("Help"));
	menu_help_->addAction(action_about_);
}

void MainWindow::CreateToolBars()
{
	toolbar_file_ = addToolBar(tr("File"));
	toolbar_file_->addAction(action_import_mesh_);
	toolbar_file_->addAction(action_export_mesh_);

	toolbar_opengl_info_ = addToolBar(tr("OpenGL"));
	toolbar_opengl_info_->addAction(action_render_points_);
	toolbar_opengl_info_->addAction(action_render_edges_);
	toolbar_opengl_info_->addAction(action_render_faces_);
	toolbar_opengl_info_->addAction(action_render_staircase_);//add for staircase 8.31.2017
	toolbar_opengl_info_->addAction(action_render_layers_);//add for layers 8-31-2017
	toolbar_opengl_info_->addAction(action_render_hexagonal_subareas_);//add for hexagonal subareas 3-8-2017
	toolbar_opengl_info_->addAction(action_render_intervals_);//add for intervals 3-14-2017
	toolbar_opengl_info_->addAction(action_render_hexagonal_subareas_parallel_hatches_);//add for hexagonal subareas parallel hatches 3-15-2017
	toolbar_opengl_info_->addAction(action_render_interval_hatches_);//add for interval hatches 3-21-2017
	toolbar_opengl_info_->addSeparator();
	toolbar_opengl_info_->addAction(action_set_background_color_);

	toolbar_mesh_status_ = addToolBar(tr("Status"));
	toolbar_mesh_status_->addAction(action_to_original_mesh_);
	toolbar_mesh_status_->addAction(action_to_noisy_mesh_);
	toolbar_mesh_status_->addAction(action_to_denoised_mesh_);
	toolbar_mesh_status_->addSeparator();
	toolbar_mesh_status_->addAction(action_clear_mesh_);
	toolbar_mesh_status_->setEnabled(false);
}

void MainWindow::CreateStatusBar()
{
	statusBar()->setStyleSheet(QString("QStatusBar::item{border: 0px}"));
	label_operation_info_ = new QLabel();
	label_operation_info_->setAlignment(Qt::AlignCenter);
	label_operation_info_->setMinimumSize(label_operation_info_->sizeHint());

	statusBar()->addWidget(label_operation_info_);
	connect(io_thread_, SIGNAL(statusShowMessage(QString)), label_operation_info_, SLOT(setText(QString)));
	connect(calculation_thread_, SIGNAL(statusShowMessage(QString)), label_operation_info_, SLOT(setText(QString)));
}

void MainWindow::CloseWidget()
{
	if (parameter_set_widget_ != NULL)
		delete parameter_set_widget_;
	parameter_set_widget_ = NULL;
}

void MainWindow::ShowWidget()
{
	calculation_thread_->initAlgorithm(data_manager_, parameter_set_);
	parameter_set_widget_ = new ParameterSetWidget(this, parameter_set_);

	connect(parameter_set_widget_, SIGNAL(ReadyToApply(QString)), this, SLOT(ApplyAlgorithms(QString)));
	addDockWidget(Qt::RightDockWidgetArea, parameter_set_widget_);
	parameter_set_widget_->showWidget();
}

void MainWindow::SetActionStatus(bool value)
{
	action_import_mesh_->setEnabled(value);
	action_export_mesh_->setEnabled(value);

	menu_algorithms_->setEnabled(value);
	menu_view_->setEnabled(value);///add 3.13.2017
	toolbar_mesh_status_->setEnabled(value);
}

void MainWindow::ImportMesh()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Import Mesh"), "../Models", tr(" *.obj *.off *.ply *.STL"));

	if (filename.isNull())
	{
		return;
	}

	io_thread_->setFileName(filename);
	io_thread_->io_type_ = ioThread::kImport;
	io_thread_->start();
}

void MainWindow::ExportMesh()
{
	QString filename = QFileDialog::getSaveFileName(this, tr("Export Mesh"), ".", tr(" CLI FILE (*.cli);; TXT FILE (*.CLI);;OBJ File (*.obj);;OFF File (*.off);;PLY File (*.ply)"));
	// add for CLI export 3.22.2017  

	if (filename.isNull())
	{
		return;
	}

	if (filename.endsWith(".cli"))
	{
		QStringList sequence_list;
		sequence_list << tr("Subarea Hatches") << tr("Subarea Contour") << tr("Interval Hatches") << tr("Layer Contour");
		//Second << tr("Subarea Contour") << tr("Interval Hatches") << tr("Layer Contour") << tr("Subarea Htaches");
		//Third  << tr("Interval Hatches") << tr("Layer Contour")<< tr("Subarea Htaches") << tr("Subarea Contour");
		//Forth  << tr("Layer Contour")<< tr("Subarea Htaches") << tr("Subarea Contour") << tr("Interval Hatches");
		bool ok;
		QString one = QInputDialog::getItem(this, tr("Set the sequence of export cli file."), tr("First"), sequence_list, 0, false, &ok);
		QString two = QInputDialog::getItem(this, tr("Set the sequence of export cli file."), tr("Second"), sequence_list, 1, false, &ok);
		QString three = QInputDialog::getItem(this, tr("Set the sequence of export cli file."), tr("Third"), sequence_list, 2, false, &ok);
		QString four = QInputDialog::getItem(this, tr("Set the sequence of export cli file."), tr("Forth"), sequence_list, 3, false, &ok);

		io_thread_->firsrt = one;
		io_thread_->second = two;
		io_thread_->third = three;
		io_thread_->forth = four;

		io_thread_->setFileName(filename);
		io_thread_->io_type_ = ioThread::kExportCLI;
	}
	else if (filename.endsWith(".CLI"))
	{

		io_thread_->setFileName(filename);
		io_thread_->io_type_ = ioThread::KExportCapitalCLI;
	}
	else
	{
		io_thread_->setFileName(filename);
		io_thread_->io_type_ = ioThread::kExport;
	}
	io_thread_->start();
}

void MainWindow::TransToNoisyMesh()
{
	data_manager_->MeshToNoisyMesh();
	opengl_viewer_->resetMesh(data_manager_->getMesh());
	opengl_viewer_->updateGL();
}

void MainWindow::TransToOriginalMesh()
{
	data_manager_->MeshToOriginalMesh();
	opengl_viewer_->resetMesh(data_manager_->getMesh());
	opengl_viewer_->updateGL();
}

void MainWindow::TransToDenoisedMesh()
{
	data_manager_->MeshToDenoisedMesh();
	opengl_viewer_->resetMesh(data_manager_->getMesh());
	opengl_viewer_->updateGL();
}

void MainWindow::ClearMesh()
{
	CloseWidget();
	data_manager_->ClearMesh();
	opengl_viewer_->resetMesh(data_manager_->getMesh());
	opengl_viewer_->updateGL();
	SetActionStatus(false);
	action_import_mesh_->setEnabled(true);
}

void MainWindow::ApplyAlgorithms(QString algorithm_name)
{
	calculation_thread_->setAlgorithmName(algorithm_name);
	calculation_thread_->start();
}

void MainWindow::ShowNoiseWidget()
{
	calculation_thread_->algorithms_type_ = CalculationThread::kNoise;
	CloseWidget();
	ShowWidget();
}

void MainWindow::ShowBilateralMeshDenoisingWidget()
{
	calculation_thread_->algorithms_type_ = CalculationThread::kBilateralMeshDenoising;
	CloseWidget();
	ShowWidget();
}

void MainWindow::ShowScanPathPlanWidget()//add for SPP,2.23.2017
{
	calculation_thread_->algorithms_type_ = CalculationThread::kScanPathPlan;
	CloseWidget();
	ShowWidget();
}

void MainWindow::ShowSimpleSPPWidget()//add for SimpleSPP,10.2.2017
{
	calculation_thread_->algorithms_type_ = CalculationThread::kSimpleSPP;
	CloseWidget();
	ShowWidget();
}

void MainWindow::ShowNonIterativeFeaturePreservingMeshFilteringWidget()
{
	calculation_thread_->algorithms_type_ = CalculationThread::kNonIterativeFeaturePreservingMeshFiltering;
	CloseWidget();
	ShowWidget();
}

void MainWindow::ShowFastAndEffectiveFeaturePreservingMeshDenoisingWidget()
{
	calculation_thread_->algorithms_type_ = CalculationThread::kFastAndEffectiveFeaturePreservingMeshDenoising;
	CloseWidget();
	ShowWidget();
}

void MainWindow::ShowBilateralNormalFilteringForMeshDenoisingWidget()
{
	calculation_thread_->algorithms_type_ = CalculationThread::kBilateralNormalFilteringForMeshDenoising;
	CloseWidget();
	ShowWidget();
}

void MainWindow::ShowMeshDenoisingViaL0MinimizationWidget()
{
	calculation_thread_->algorithms_type_ = CalculationThread::kMeshDenoisingViaL0Minimization;
	CloseWidget();
	ShowWidget();
}

void MainWindow::ShowGuidedMeshNormalFilteringWidget()
{
	calculation_thread_->algorithms_type_ = CalculationThread::kGuidedMeshNormalFiltering;
	CloseWidget();
	ShowWidget();
}

void MainWindow::setActionAndWidget(bool value1, bool value2)
{
	SetActionStatus(value1);

	if (parameter_set_widget_ != NULL && value2){
		CloseWidget();
		ShowWidget();
	}
}

void MainWindow::needToUpdateGL(bool value)
{
	opengl_viewer_->resetMesh(data_manager_->getMesh(), value);
	opengl_viewer_->updateGL();
}

void MainWindow::ShowSingleLayer()//add  3.13.2017
{
	bool judge;
	int num = QInputDialog::getInt(this, tr("Show the layer"), tr("Input the layer's number"), QLineEdit::Normal, 0, 1000, 1, &judge);
	if (judge)
	{
		opengl_viewer_->examiner_->setShowSingleLayerNumber(num);
	}
}

void MainWindow::About()
{
	QMessageBox::about(this, tr("About MeshDenoise UI"),
		tr("MeshDenoise UI Version 1.0. <br>"
		"The UI is based on Qt, OpenMesh and Eigen Library. <br>"
		"<br>"
		"Mesh denoising algorithms included are <br>"
		"<a href='http://courses.cs.tau.ac.il/~dcor/articles/2003/Bilateral-Mesh-Denoising.pdf'>Bilateral Mesh Denoising</a>, <br>"
		"<a href='http://people.csail.mit.edu/thouis/JDD03.pdf'>Non-Iterative, Feature-Preserving Mesh Smoothing</a>, <br>"
		"<a href='http://ralph.cs.cf.ac.uk/papers/Geometry/MeshDenoising.pdf'>Fast and Effective Feature-Preserving Mesh Denoising</a>, <br>"
		"<a href='http://www.cs.ust.hk/~taicl/papers/TVCG-denoising.pdf'>Bilateral Normal Filtering for Mesh Denoising</a>, <br>"
		"<a href='http://faculty.cs.tamu.edu/schaefer/research/L0Smoothing.pdf'>Mesh Denoising via L0 Minimization</a>, <br>"
		"<a href='http://staff.ustc.edu.cn/~juyong/Papers/GuidedFilter.pdf'>Guided Mesh Normal Filtering</a>. <br>"
		"<br>"
		"If you have any question, please contact <br>"
		"<a href='mailto:zhwangyu@mail.ustc.edu.cn'>zhwangyu@mail.ustc.edu.cn</a>."));
}
