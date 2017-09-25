#ifndef SLICE_H
#define SLICE_H

#include "datamanager.h"
#include "Clipper/clipper.hpp"
// Define the data structure of COUNTER
//typedef std::vector<TriMesh::Point> Polylines;
//typedef std::vector<Polylines> Contours;
class Slice
{
public:
	Slice();
	~Slice();

public:
	void GetMaxMinZ(TriMesh &mesh, double &max_z, double &min_z);
	double GetLayersNumber(TriMesh &mesh, double &thickness, double &max_z, double &min_z);//get the number of layers	
	bool IsTheEdgeIntersectWithTheLayer(TriMesh &mesh, TriMesh::EdgeHandle eh, double &height);//judge weather the edge is intersected  the layer or not.
	void GetTheIntersectionPoints(TriMesh &mesh, TriMesh::HalfedgeHandle heh, double &height, std::vector<TriMesh::Point> &inter_points);//get the intersection point between the halfedge and the layer
	void GetContour(TriMesh &mesh, std::vector<int> &edges_status, TriMesh::HalfedgeHandle heh, double &height, TriMesh::Polylines &contour);//get contour from a intersected halfedge.
	void SliceTheModel(TriMesh &mesh);

public:
	//double Dot(TriMesh::Normal &a, TriMesh::Point &b);

public:
	DataManager *data_manager;

	double thickness;
	int layers_number;
	TriMesh::Slicing slicing;
	double model_max_z = 1.0e-9;
	double model_min_z = 1.0e8;

//////////////////////////////////////////////////////////////////////////
//add for triangles between 2 layers, 9.20.2017
public:
	
	void getFacetMaxAndMinZ(TriMesh &mesh, TriMesh::FaceIter f_it, double &maximal_z_, double &minimal_z_);
	void sliceTheModelWithThinkingAboutTrianglesBetween2Layers(TriMesh &mesh, std::vector<Paths> &volume_offset_layers_integer,int &scale);
	void getIntersectionIntergerPointOfOneFacetWith2Layers(TriMesh &mesh, TriMesh::FaceIter &f_it, double &layer_z, double &next_layer_z, int &scale, Path &projection_intersection_polygon_);
	bool isTheEdgeIntersectedwithThelayer(double &frome_z,double &to_z, double &height);
	void getEdgesIntersectionPointsWithOneLayer(TriMesh::Point &to_vertex, TriMesh::Point &from_vertex, double &height_1_, double &height_2_, std::vector<TriMesh::Point> &intersection_points);
	void getEdgesIntersectionPointsWithTwoLayers(TriMesh::Point &to_vertex, TriMesh::Point &from_vertex, double &height_1_, double &height_2_, std::vector<TriMesh::Point> &intersection_points);
	void getEdgesIntersectionPointWithOneLayer(TriMesh::Point &to_vertex, TriMesh::Point &from_vertex, double &height, TriMesh::Point &intersection_point);
};

#endif // !SLICE_H

