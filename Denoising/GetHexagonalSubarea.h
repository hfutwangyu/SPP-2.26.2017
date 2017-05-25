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

	void getOuterContoursofEachLayer(TriMesh::Slicing &slice_of_mesh_);
	void segmenLayersIntoHexagonalSubareasWithOuterBoundryOffset(TriMesh &mesh, TriMesh::Slicing &slice_of_mesh_);//only offseted outer contour 5.2.2017
    void getOffsettedOuterContours(TriMesh::Slicing &slice_of_mesh_);//5.2.2017
   
	void OffsetForAllLayerContours(TriMesh::Slicing &slice_of_mesh_);//5.4.2017 make all layers' contours OFFSETTED 

	void offsetIntervalContoursWithHexagonSubsectors(Paths &contours_integer, Paths &a_layer_solution_intersection_, Paths &layer_solution_difference_with_offseted_subsectors_);//5.19.2017
	void offsetIntervalContoursWithIntervals(Paths &contours_integer, Paths &a_layer_solution_difference_, Paths &layer_solution_difference_with_offsected_intervals_);//5.19.2017
public:
	std::vector<Paths> hexagons_in_layers_interger_;
	std::vector<Paths> interior_hexagons_in_layers_interger_;//5.16.2017
	std::vector<Paths> layers_integer_;
	std::vector<Paths> offsetted_layers_integer_;
	std::vector<Paths> intervals_with_negative_offseted_subsectors_;//5.18.2017 for interval hatches crossing subsectors

	std::vector<Paths> layer_outer_contours_;

	std::vector<std::vector<int>> temp_hexagon_column_order_;//5.12.2017
	std::vector<std::vector<int>> hexagon_column_order_;//5.12.2017
	double side_length_of_bounding_hexagon=4.0;
	double side_length_of_hexagon=3.0;
    int scale = 10000;
	double offset=0.1;
	double parallel_line_spacing = 0.4;
};

#endif // HEXAGONALSUBAREA_H


