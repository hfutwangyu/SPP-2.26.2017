#include "GetHexagonHatches.h"


GetHexagonHatches::GetHexagonHatches() :parallel_line_spacing(0.03)
{
}


GetHexagonHatches::~GetHexagonHatches()
{
}


void GetHexagonHatches::getParallelLines(TriMesh::Slicing &slice_of_mesh_, int &scale)
{
	for (auto contours_it = slice_of_mesh_.begin(); contours_it != slice_of_mesh_.end(); contours_it++)
	{
		double min_x = 1.0e8, max_x = 1.0e-9, min_y = 1.0e8, max_y = 1.0e-9;
		TriMesh::Contours layer_contours = *contours_it;
		GetHexagonalSubarea get_mesh_hexagonal_subarea_;
		get_mesh_hexagonal_subarea_.getMaxAndMinXYofLayer(layer_contours, min_x, max_x, min_y, max_y);

		double horizontal_min = min_x - 1.0,
			   horizontal_max = max_x + 1.0,
			   vertical_y = 0.0,
			   vertical_max = max_y;
		Paths lines;
		for (int i = 0; (vertical_y = min_y + i*parallel_line_spacing) <= vertical_max; i++)
		{
			Path line;
			line << IntPoint(horizontal_min*scale,vertical_y*scale)
				<< IntPoint(horizontal_max*scale,vertical_y*scale);
			lines << line;
		}
		parallel_lines_.push_back(lines);
	}
}


void GetHexagonHatches::getHexagonHatchesLines(TriMesh &mesh)
{
	for (int i = 0;i!= parallel_lines_.size(); i++)
	{

		Paths lines = parallel_lines_[i];
		std::vector<Paths> layer_solution;
		for (int j = 0; j < mesh.mesh_hexagoned_hexagons_int_paths_[i].size();j++)
		{
			Path p = mesh.mesh_hexagoned_hexagons_int_paths_[i][j];
			Clipper c;
		    PolyTree solution;
			c.AddPaths(lines, ptSubject, false);
			c.AddPath(p, ptClip, true);
			c.Execute(ctIntersection, solution, pftEvenOdd, pftEvenOdd);

			Paths ps;
			OpenPathsFromPolyTree(solution, ps);
			layer_solution.push_back(ps);
		}
		
		mesh.mesh_hexagon_hatches_int_.push_back(layer_solution);
	}
}
