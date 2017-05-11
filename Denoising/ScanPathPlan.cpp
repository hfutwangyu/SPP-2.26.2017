#include "ScanPathPlan.h"


ScanPathPlan::ScanPathPlan(DataManager *_data_manager, ParameterSet *_parameter_set)
: MeshDenoisingBase(_data_manager, _parameter_set)
{
	initParameters();
}


ScanPathPlan::~ScanPathPlan()
{
}

void ScanPathPlan::denoise()
{
	TriMesh mesh = data_manager_->getNoisyMesh();
	Slice slice_of_date_manager;
	parameter_set_->getValue(QString("Layer Thickness"), slice_of_date_manager.thickness);
	slice_of_date_manager.SliceTheModel(mesh);
	mesh.mesh_slicing_ = slice_of_date_manager.slicing;//SET THE SLICING DATA OF THE MESH
	getSlicedLayers(mesh,slice_of_date_manager);//get each layer's position 3.22.2017
	GetHexagonalSubarea get_mesh_hexagonal_subarea_;
	parameter_set_->getValue(QString("Side Length of Rounding hexagon"), get_mesh_hexagonal_subarea_.side_length_of_bounding_hexagon);
	parameter_set_->getValue(QString("Side Length of hexagonal subarea"), get_mesh_hexagonal_subarea_.side_length_of_hexagon);
	parameter_set_->getValue(QString("Transformation ration between double data and cInt data"), get_mesh_hexagonal_subarea_.scale);
	parameter_set_->getValue(QString("Offset"), get_mesh_hexagonal_subarea_.offset);
	//get_mesh_hexagonal_subarea_.segmenLayersIntoHexagonalSubareas(mesh, mesh.mesh_slicing_);//get hexagonal subareas with cInt data type
	get_mesh_hexagonal_subarea_.segmenLayersIntoHexagonalSubareasWithOuterBoundryOffset(mesh, mesh.mesh_slicing_);//get hexagonal subareas with offsetted boundary 5.2.2017
    get_mesh_hexagonal_subarea_.getLayerContoursOrientation(mesh, slice_of_date_manager.slicing);
	get_mesh_hexagonal_subarea_.layers_integer_.swap(std::vector<Paths> ());//free the memory 3.23.2017
	get_mesh_hexagonal_subarea_.hexagons_in_layers_interger_.swap(std::vector<Paths>());//free the memory 3.23.2017

	GetHexagonHatches get_hexagon_hatches_;//get parallel line hatches of hexagonal subares
	parameter_set_->getValue(QString("Parallel Line Spacing of Hexagonal Subareas"), get_hexagon_hatches_.parallel_line_spacing);
/*	
    // undirectional hatches
	get_hexagon_hatches_.getParallelLines(mesh.mesh_slicing_, get_mesh_hexagonal_subarea_.scale);
*/
	// triangular parallel hatches between adjoint 3 layers 4.17.2017
	get_hexagon_hatches_.getTriangularParallelLinesBetweenLayers(mesh.mesh_slicing_, get_mesh_hexagonal_subarea_.scale);
	get_hexagon_hatches_.getHexagonHatchesLines(mesh);
	transformMeshSegmentedSlicingfromCIntToDouble(mesh, slice_of_date_manager, get_mesh_hexagonal_subarea_);//change the data type from CInt to double
	transformHexagonaHatchesFromCIntToDouble(mesh, slice_of_date_manager, get_mesh_hexagonal_subarea_);//change the data type from CInt to double
	get_hexagon_hatches_.parallel_lines_.swap(std::vector<Paths> ());//free the memory 3.23.2017
	
	GetIntervalHatches get_interval_hatches_;
	parameter_set_->getValue(QString("Parallel Line Spacing of Intervals"), get_interval_hatches_.parallel_line_spacing);
	get_interval_hatches_.get0ParallelLines(mesh.mesh_slicing_, get_mesh_hexagonal_subarea_.scale);
	get_interval_hatches_.get60ParallelLines(mesh.mesh_slicing_, get_mesh_hexagonal_subarea_.scale);
	get_interval_hatches_.get120ParallelLines(mesh.mesh_slicing_, get_mesh_hexagonal_subarea_.scale);
	get_interval_hatches_.getIntervalHatchTriangles(mesh);
	get_interval_hatches_._0_parallel_lines_.swap(std::vector<Paths>());//free the memory 3.23.2017
	get_interval_hatches_._60_parallel_lines_.swap(std::vector<Paths>());//free the memory 3.23.2017
	get_interval_hatches_._120_parallel_lines_.swap(std::vector<Paths>());//free the memory 3.23.2017

	transformIntervalContoursfromCIntToDouble(mesh, slice_of_date_manager, get_mesh_hexagonal_subarea_);
	transformIntervalsHatchesFromCIntToDouble(mesh, slice_of_date_manager, get_mesh_hexagonal_subarea_);//change the data type from CInt to double
	
	//transformMeshSegmentedSlicingfromCIntToDouble(mesh,slice_of_date_manager,get_mesh_hexagonal_subarea_);//change the data type from CInt to double
   //transformHexagonaHatchesFromCIntToDouble(mesh, slice_of_date_manager, get_mesh_hexagonal_subarea_);//change the data type from CInt to double
	//transformIntervalsHatchesFromCIntToDouble(mesh, slice_of_date_manager, get_mesh_hexagonal_subarea_);//change the data type from CInt to double
	data_manager_->setMesh(mesh);
	data_manager_->setDenoisedMesh(mesh);
}

void ScanPathPlan::getPlanPath()
{

}

void ScanPathPlan::initParameters()
{
	parameter_set_->removeAllParameter();
	parameter_set_->addParameter(QString("Layer Thickness"), 1.1, QString("Layer Thickness ="), QString("The layer thickness of the sliced model."),
		true, 1.0e-9, 1.0e8);
	parameter_set_->addParameter(QString("Side Length of Rounding hexagon"), 4.0, QString("SL of RH ="), QString("The side length of rounding hexagon."),
		true, 1.0e-9, 1.0e8);
	parameter_set_->addParameter(QString("Side Length of hexagonal subarea"), 3.0, QString("SL of HS ="), QString("The side length of hexagonal subarea, less than SL of RH."),
		true, 1.0e-9, 1.0e8);
	parameter_set_->addParameter(QString("Transformation ration between double data and cInt data"), 1000, QString("Scale ="), QString("Double data multiply scale to get cInt data."),
		true, 1, 100000000000);
	parameter_set_->addParameter(QString("Parallel Line Spacing of Hexagonal Subareas"), 0.03, QString("PLS of HS ="), QString("Parallel Line Spacing of Hexagonal Subareas"), 
		true, 1.0e-9, 1.0e8);
	parameter_set_->addParameter(QString("Parallel Line Spacing of Intervals"), 0.03, QString("PLS of Intervals ="), QString("Parallel Line Spacing of Intervals"),
		true, 1.0e-9, 1.0e8);
	parameter_set_->addParameter(QString("Offset"), 0.2, QString("Offset ="), QString("Contours' Offset"),
		true, 1.0e-9, 1.0e8);
	parameter_set_->setName(QString("SPP"));
	parameter_set_->setLabel(QString("SPP"));
	parameter_set_->setIntroduction(QString("SPP -- Parameters"));
}

void ScanPathPlan::transformMeshSegmentedSlicingfromCIntToDouble(TriMesh &mesh, Slice &slice_of_date_manager, GetHexagonalSubarea &get_mesh_hexagonal_subarea_)
{
	for (int i = 0; i < mesh.mesh_hexagoned_hexagons_int_paths_.size(); i++)//segment layers into hexagonal subareas
	{
		double z = (slice_of_date_manager.model_min_z + (i + 1)*(slice_of_date_manager.thickness));
		std::vector<Paths> a_layer_subareas_paths_ = mesh.mesh_hexagoned_hexagons_int_paths_[i];
		TriMesh::SegmentedLayers mesh_segmented_layers;
		for (int j = 0; j < a_layer_subareas_paths_.size(); j++)
		{
			Paths a_subarea_paths_ = a_layer_subareas_paths_[j];
			TriMesh::Subareas sub_hexagon_areas;
			for (int k = 0; k < a_subarea_paths_.size(); k++)
			{
				Path a_path = a_subarea_paths_[k];
				TriMesh::Polylines a_subarea_contour_;
				for (int l = 0; l < a_path.size();l++)
				{
					IntPoint int_pt = a_path[l];
					TriMesh::Point mesh_point;
					mesh_point[0] = (double)(int_pt.X) / (get_mesh_hexagonal_subarea_.scale);
					mesh_point[1] = (double)(int_pt.Y) / (get_mesh_hexagonal_subarea_.scale);
					mesh_point[2] = z;

					a_subarea_contour_.push_back(mesh_point);
				}
				sub_hexagon_areas.push_back(a_subarea_contour_);
			}
			mesh_segmented_layers.push_back(sub_hexagon_areas);
		}
		mesh.mesh_segmented_slicing.push_back(mesh_segmented_layers);
	}

	mesh.mesh_hexagoned_hexagons_int_paths_.swap(std::vector<std::vector<Paths>> ());//free the memory 3.23.2017
	//mesh.mesh_hexagoned_hexagons_int_paths_.resize(1);//free the memory 3.23.2017
}


void ScanPathPlan::transformHexagonaHatchesFromCIntToDouble(TriMesh &mesh, Slice &slice_of_date_manager, GetHexagonalSubarea &get_mesh_hexagonal_subarea_)
{
	for (int i = 0; i < mesh.mesh_hexagon_hatches_int_.size();i++)
	{
		double z = (slice_of_date_manager.model_min_z + (i + 1)*(slice_of_date_manager.thickness));
		std::vector<Paths> ps_layer = mesh.mesh_hexagon_hatches_int_[i];
		TriMesh::HatchesForOneLayer hatches_of_a_layer;
		for (int j = 0; j < ps_layer.size();j++)
		{
			Paths ps = ps_layer[j];
			TriMesh::HatchesForOneHexagonSubarea hatches_of_a_subarea;
			for (int k = 0; k < ps.size();k++)
			{
				Path P = ps[k];
				TriMesh::Segment seg;
				for (int n = 0; n < P.size();n++)
				{
					IntPoint ipt = P[n];
					TriMesh::Point pt;
					pt[0] = (double)(ipt.X) / (get_mesh_hexagonal_subarea_.scale);
					pt[1] = (double)(ipt.Y) / (get_mesh_hexagonal_subarea_.scale);
					pt[2] = z;	
					seg.push_back(pt);
				}
				hatches_of_a_subarea.push_back(seg);
			}
			hatches_of_a_layer.push_back(hatches_of_a_subarea);
		}
		mesh.mesh_hexagon_hatches_double_.push_back(hatches_of_a_layer);
	}

	mesh.mesh_hexagon_hatches_int_.swap(std::vector<std::vector<Paths>> ());////free the memory 2.23.2017
	//mesh.mesh_hexagon_hatches_int_.resize(1);////free the memory 2.23.2017

}


void ScanPathPlan::transformIntervalsHatchesFromCIntToDouble(TriMesh &mesh, Slice &slice_of_date_manager, GetHexagonalSubarea &get_mesh_hexagonal_subarea_)
{
	for (int i = 0; i < mesh.mesh_interval_hatches_int_.size();i++)
	{
		double z = (slice_of_date_manager.model_min_z + (i + 1)*(slice_of_date_manager.thickness));
		std::vector<Paths> ps_layer = mesh.mesh_interval_hatches_int_[i];
		TriMesh::HatchesForOneLayersInterval interval_hatches_of_a_layer;
		for (int j = 0; j < ps_layer.size();j++)
		{
			Paths ps = ps_layer[j];
			for (int k = 0; k < ps.size();k++)
			{
				Path p = ps[k];
				TriMesh::Segment seg;
				for (int n = 0; n < p.size(); n++)
				{
					IntPoint ipt = p[n];
					TriMesh::Point pt;
					pt[0] = (double)(ipt.X) / (get_mesh_hexagonal_subarea_.scale);
					pt[1] = (double)(ipt.Y) / (get_mesh_hexagonal_subarea_.scale);
					pt[2] = z;
					seg.push_back(pt);
				}
				interval_hatches_of_a_layer.push_back(seg);
			}
		}
		mesh.mesh_interval_hatches_double_.push_back(interval_hatches_of_a_layer);
	}

	mesh.mesh_interval_hatches_int_.swap(std::vector<std::vector<Paths>> ());//////free the memory 2.23.2017
	//mesh.mesh_interval_hatches_int_.resize(1);//////free the memory 2.23.2017
}


void ScanPathPlan::getSlicedLayers(TriMesh &mesh, Slice &slice_of_date_manager)
{
	for (int i = 0; i < slice_of_date_manager.slicing.size(); i++)
	{
		double z = (slice_of_date_manager.model_min_z + (i + 1)*(slice_of_date_manager.thickness));
		mesh.mesh_layers_.push_back(z);
	}
}


void ScanPathPlan::transformIntervalContoursfromCIntToDouble(TriMesh &mesh, Slice &slice_of_date_manager, GetHexagonalSubarea &get_mesh_hexagonal_subarea_)
{
	for (int i = 0; i < mesh.mesh_areas_betweent_hexagons_int_paths_.size(); i++)//intervals between hexagonal subareas
	{
		double z = (slice_of_date_manager.model_min_z + (i + 1)*(slice_of_date_manager.thickness));
		Paths ps = mesh.mesh_areas_betweent_hexagons_int_paths_[i];
		TriMesh::BetweenSegmentedLayers mesh_between_segmented_layers;
		for (int j = 0; j < ps.size(); j++)
		{
			Path p = ps[j];
			TriMesh::BetweenSubareas between_hexagon_sunareas_;
			for (int k = 0; k < p.size(); k++)
			{
				IntPoint int_pt = p[k];
				TriMesh::Point mesh_point;
				mesh_point[0] = (double)(int_pt.X) / (get_mesh_hexagonal_subarea_.scale);
				mesh_point[1] = (double)(int_pt.Y) / (get_mesh_hexagonal_subarea_.scale);
				mesh_point[2] = z;
				between_hexagon_sunareas_.push_back(mesh_point);
			}
			mesh_between_segmented_layers.push_back(between_hexagon_sunareas_);
		}
		mesh.mesh_between_segmented_slicing_.push_back(mesh_between_segmented_layers);
	}

	mesh.mesh_areas_betweent_hexagons_int_paths_.swap(std::vector<Paths> ());//free the memory 2.23.2017
	//mesh.mesh_areas_betweent_hexagons_int_paths_.resize(1);//free the memory 2.23.2017
}
