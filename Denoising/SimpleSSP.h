#ifndef SIMPLESPP_H
#define SIMPLESPP_H

#include "../Algorithms/MeshDenoisingBase.h"
#include "Slice.h"
//#include "GetHexagonalSubarea.h"
//#include "GetHexagonHatches.h"
#include "GetIntervalHatches.h"
class SimpleSSP : public MeshDenoisingBase
{
public:
	SimpleSSP(DataManager *_data_manager, ParameterSet *_parameter_set);
	~SimpleSSP();

private:
	void denoise();
	void initParameters();
public:
	void getSlicedLayers(TriMesh &mesh, Slice &slice_of_date_manager);
	int scale;
	std::vector<Paths> layers_integer_;
	void transformLayersDataTypeToInteger(TriMesh::Slicing &slice_of_mesh_);
	void getParallelLines(TriMesh::Slicing &slice_of_mesh_, int &scale);
	double parallel_line_spacing;
	std::vector<Paths> un_intersected_hatch_lines;
	void getTriangularParallelLinesBetweenLayers(TriMesh::Slicing &slice_of_mesh_, int &scale);
	void getHatchesLines();
	std::vector<std::vector<Path>>       hatchesInt;
	void transformHatchesFromCIntToDouble(TriMesh &mesh, Slice &slice_of_date_manager);
	void getLayerContoursOrientation(TriMesh &mesh, TriMesh::Slicing &slice_of_mesh_);
	void getMaxAndMinXYofLayer(TriMesh::Contours &layer_contours, double &min_x, double &max_x, double &min_y, double &max_y);
};

#endif // SIMPLESPP_H