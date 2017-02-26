#ifndef SLICE_H
#define SLICE_H

#include "datamanager.h"

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
	bool IsIntersect(TriMesh &mesh, TriMesh::EdgeHandle eh, double &height);//judge weather the edge is intersected  the layer or not.
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


};

#endif // !SLICE_H

