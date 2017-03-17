#ifndef GETHEXAGONHATCHES
#define GETHEXAGONHATCHES

#include "GetHexagonalSubarea.h"
class GetHexagonHatches
{
public:
	GetHexagonHatches();
	~GetHexagonHatches();

public:
	void getParallelLines(TriMesh::Slicing &slice_of_mesh_, int &scale);
	void getHexagonHatchesLines(TriMesh &mesh);
public:
	double parallel_line_spacing;
	std::vector<Paths> parallel_lines_;
	
};

#endif // GETHEXAGONHATCHES