#ifndef GETHEXAGONHATCHES
#define GETHEXAGONHATCHES

#include "GetHexagonalSubarea.h"
class GetHexagonHatches
{
public:
	GetHexagonHatches();
	~GetHexagonHatches();

public:
	// undirectional hatches
	void getParallelLines(TriMesh::Slicing &slice_of_mesh_, int &scale);
	void getHexagonHatchesLines(TriMesh &mesh);
	// triangular parallel hatches between adjoint 3 layers 4.17.2017
	void getTriangularParallelLinesBetweenLayers(TriMesh::Slicing &slice_of_mesh_, int &scale);
	
	void get60ParallelLines(TriMesh::Slicing &slice_of_mesh_, int &scale);//add  5.11.2017
	void get120ParallelLines(TriMesh::Slicing &slice_of_mesh_, int &scale);//add  5.11.2017
	void get0ParallelLines(TriMesh::Slicing &slice_of_mesh_, int &scale);//add  5.11.2017
	void getHexagonHatchesLinesWithTriangularParallelLinesBetween3Columns(TriMesh &mesh, std::vector<std::vector<int>> &hexagon_column_order_);//5.12.2017

public:
	double parallel_line_spacing;
	std::vector<Paths> un_intersected_subsector_hatch_lines;//for intersection with subsectors
	
	std::vector<Paths> _0_parallel_lines_;//add 5.11.2017
	std::vector<Paths> _60_parallel_lines_;//add  5.11.2017
	std::vector<Paths> _120_parallel_lines_;//add  5.11.2017
	
};

#endif // GETHEXAGONHATCHES