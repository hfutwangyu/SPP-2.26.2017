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
	

public:
	double parallel_line_spacing;
	std::vector<Paths> parallel_lines_;
	
	
};

#endif // GETHEXAGONHATCHES