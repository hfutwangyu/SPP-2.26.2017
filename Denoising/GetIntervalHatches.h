#ifndef GETINTERVALHATCHES
#define GETINTERVALHATCHES

#include "GetHexagonHatches.h"

class GetIntervalHatches
{
public:
	GetIntervalHatches();
	~GetIntervalHatches();

public:
	void get60ParallelLines(TriMesh::Slicing &slice_of_mesh_, int &scale);
	void get120ParallelLines(TriMesh::Slicing &slice_of_mesh_, int &scale);
	void get0ParallelLines(TriMesh::Slicing &slice_of_mesh_, int &scale);
    void getIntervalHatchTriangles(TriMesh &mesh);

	void getIntervalHatchYConnection(TriMesh &mesh, std::vector<Paths> &layers_integer_, std::vector<Paths> &bounding_hexagons_in_layers_interger_);//add Y Connection 5.16.2017
	void getY60ParallelLines(TriMesh::Slicing &slice_of_mesh_, int &scale, double &side_length_of_bounding_hexagon);//5.16.2017
	void getY120ParallelLines(TriMesh::Slicing &slice_of_mesh_, int &scale, double &side_length_of_bounding_hexagon);//5.16.2017
	void getY0ParallelLines(TriMesh::Slicing &slice_of_mesh_, int &scale, double &side_length_of_bounding_hexagon);//5.16.2017

	void getIntervalHatchTrianglesCrossedSubsectors(TriMesh &mesh, std::vector<Paths> &intervals_with_negative_offseted_subsectors_);//5.18.2017
public:
	double parallel_line_spacing;
	std::vector<Paths> _0_parallel_lines_, _60_parallel_lines_, _120_parallel_lines_;
	std::vector<Paths> Y_0_parallel_lines_, Y_60_parallel_lines_, Y_120_parallel_lines_;//5.16.2017
	
	

};

#endif // !GETINTERVALHATCHES


