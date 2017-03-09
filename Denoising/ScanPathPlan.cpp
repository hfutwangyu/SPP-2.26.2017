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
	GetHexagonalSubarea get_mesh_hexagonal_subarea_;
	parameter_set_->getValue(QString("Side Length of Rounding hexagon"), get_mesh_hexagonal_subarea_.side_length_of_bounding_hexagon);
	parameter_set_->getValue(QString("Side Length of hexagonal subarea"), get_mesh_hexagonal_subarea_.side_length_of_hexagon);
	parameter_set_->getValue(QString("Transformation ration between double data and cInt data"), get_mesh_hexagonal_subarea_.scale);
	get_mesh_hexagonal_subarea_.segmenLayersIntoHexagonalSubareas(mesh, mesh.mesh_slicing_);//get hexagonal subareas

	for (int i = 0; i < mesh.mesh_hexagoned_hexagons_int_paths_.size();i++)
	{
		double z = (slice_of_date_manager.model_min_z+(i+1)*(slice_of_date_manager.thickness));
		Paths ps = mesh.mesh_hexagoned_hexagons_int_paths_[i];
		TriMesh::SegmentedLayers mesh_segmented_layers;
		for (int j = 0; j < ps.size();j++)
		{
			Path p = ps[j];	
			TriMesh::Subareas sub_hexagon_areas;
			for (int k = 0; k < p.size();k++)
			{
				IntPoint int_pt = p[k];
				TriMesh::Point mesh_point;
				mesh_point[0] = (double)(int_pt.X) / (get_mesh_hexagonal_subarea_.scale);
				mesh_point[1] = (double)(int_pt.Y) / (get_mesh_hexagonal_subarea_.scale);
				mesh_point[2] = z;
				
				sub_hexagon_areas.push_back(mesh_point);
			}
			mesh_segmented_layers.push_back(sub_hexagon_areas);
		}
		mesh.mesh_segmented_slicing.push_back(mesh_segmented_layers);
	}

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
	parameter_set_->setName(QString("SPP"));
	parameter_set_->setLabel(QString("SPP"));
	parameter_set_->setIntroduction(QString("SPP -- Paramters"));
}