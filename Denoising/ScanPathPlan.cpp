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
	parameter_set_->setName(QString("SPP"));
	parameter_set_->setLabel(QString("SPP"));
	parameter_set_->setIntroduction(QString("SPP -- Paramters"));
}