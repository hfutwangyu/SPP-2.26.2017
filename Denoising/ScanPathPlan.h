#ifndef SCANPATHPLAN_H
#define SCANPATHPLAN_H

#include "../Algorithms/MeshDenoisingBase.h"
//#include "Slice.h"
//#include "GetHexagonalSubarea.h"
#include "GetHexagonHatches.h"
class ScanPathPlan : public MeshDenoisingBase
{
public:
	ScanPathPlan(DataManager *_data_manager, ParameterSet *_parameter_set);
	~ScanPathPlan();
private:
	void denoise();
	void initParameters();

	void getPlanPath();
public:
	void transformMeshSegmentedSlicingfromCIntToDouble(TriMesh &mesh, Slice &slice_of_date_manager, GetHexagonalSubarea &get_mesh_hexagonal_subarea_);
	void transformHexagonaHatchesFromCIntToDouble(TriMesh &mesh, Slice &slice_of_date_manager, GetHexagonalSubarea &get_mesh_hexagonal_subarea_);
};

#endif // SCANPATHPLAN_H