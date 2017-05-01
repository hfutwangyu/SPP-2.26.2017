#ifndef HEXAGONALSUBAREA_H
#define HEXAGONALSUBAREA_H

#include "Slice.h"
#include "Clipper/clipper.hpp"
using namespace ClipperLib;
class GetHexagonalSubarea
{
public:
	GetHexagonalSubarea();
	~GetHexagonalSubarea();

public:
	void transformLayersDataTypeToInteger(TriMesh::Slicing &slice_of_mesh_);
    void getMaxAndMinXYofLayer(TriMesh::Contours &layer_contours, double &min_x, double &max_x, double &min_y,double &max_y);
	void getHexagons(TriMesh::Slicing &slice_of_mesh_);//get hexagons from slicing layers
	void getHexagonsStaggeredBetweenLayers(TriMesh::Slicing &slice_of_mesh_);//get hexagons starggered between layers 4.22.2017
    void segmenLayersIntoHexagonalSubareas(TriMesh &mesh, TriMesh::Slicing &slice_of_mesh_);
	void getLayerContoursOrientation(TriMesh &mesh, TriMesh::Slicing &slice_of_mesh_);

public:
	std::vector<Paths> hexagons_in_layers_interger_;
	std::vector<Paths> layers_integer_;

	double side_length_of_bounding_hexagon=4.0;
	double side_length_of_hexagon=3.0;
    int scale = 10000;
	
};

#endif // HEXAGONALSUBAREA_H


