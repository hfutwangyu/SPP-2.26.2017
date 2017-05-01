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
		
	/*	
		    Paths p = mesh.mesh_hexagoned_hexagons_int_paths_[i];
			Clipper c;
		    PolyTree solution;
			c.AddPaths(lines, ptSubject, false);
			c.AddPaths(p, ptClip, true);
			c.Execute(ctIntersection, solution, pftEvenOdd, pftEvenOdd);

			Paths ps;
			OpenPathsFromPolyTree(solution, ps);
			layer_solution.push_back(ps);
	*///get a layer's subareas' hatches through one intersection//
		for (int j = 0; j < mesh.mesh_hexagoned_hexagons_int_paths_[i].size();j++)
		{
			Paths ps_ = mesh.mesh_hexagoned_hexagons_int_paths_[i][j];
			Clipper c;
			PolyTree solution;
			c.AddPaths(lines, ptSubject, false);
			c.AddPaths(ps_, ptClip, true);
			c.Execute(ctIntersection, solution, pftEvenOdd, pftEvenOdd);

			Paths ps;
			OpenPathsFromPolyTree(solution, ps);
			if (ps.size()!=0)
			{
				layer_solution.push_back(ps);
			}
		}
		mesh.mesh_hexagon_hatches_int_.push_back(layer_solution);
	}
}


void GetHexagonHatches::getTriangularParallelLinesBetweenLayers(TriMesh::Slicing &slice_of_mesh_, int &scale)
// // triangular parallel hatches between adjoint 3 layers 4.17.2017
{
	int n_layers = slice_of_mesh_.size();
	for (int i = 0; i < n_layers; i+=3)
	{
		double min_x = 1.0e8, max_x = 1.0e-9, min_y = 1.0e8, max_y = 1.0e-9;
		

	    //if ((i<n_layers)&&((i+1) % 1 == 0))// 0 degree parallel hatch lines 
		if (i<n_layers)
		{
			TriMesh::Contours layer_contours = slice_of_mesh_[i];
			GetHexagonalSubarea get_mesh_hexagonal_subarea_1;
			get_mesh_hexagonal_subarea_1.getMaxAndMinXYofLayer(layer_contours, min_x, max_x, min_y, max_y);

			double horizontal_min = min_x - 1.0,
				horizontal_max = max_x + 1.0,
				vertical_y = 0.0,
				vertical_min = min_y;
			Paths lines;
			//for (int i = 0; (vertical_y =  max_y + (max_x - min_x)*sqrt(3.0) - i*parallel_line_spacing) >= vertical_min; i++)
			for (int i = 0; (vertical_y = max_y + (max_x - min_x)*sqrt(3.0) - i*sqrt(3.0)*parallel_line_spacing / 2.0) >= vertical_min; i++)
			{
				Path line;
				line << IntPoint(horizontal_min*scale, vertical_y*scale)
					<< IntPoint(horizontal_max*scale, vertical_y*scale);
				lines << line;
			}
			parallel_lines_.push_back(lines);
		} 

		
		//if (((i + 1)<n_layers) && ((i + 2) % 2 == 0)) // 60 degree parallel hatch lines 
		if ((i + 1)<n_layers)
		{
			TriMesh::Contours layer_contours = slice_of_mesh_[i + 1];
			GetHexagonalSubarea get_mesh_hexagonal_subarea_2;
			get_mesh_hexagonal_subarea_2.getMaxAndMinXYofLayer(layer_contours, min_x, max_x, min_y, max_y);

			double //start_x = (max_y-min_y+1.0)/sqrt(3.0)+2*max_x-min_x,
				start_x = 0.0,
				start_y = min_y - 1.0,
				end_x = max_x,
				end_y = max_y + (max_x - min_x)*sqrt(3.0);
			Paths lines;
			//for (int i = 0; (start_x = (max_y - min_y + 1.0) / sqrt(3.0) + 2 * max_x - min_x - 2.0*i*parallel_line_spacing / sqrt(3.0)) >= min_x; i++)
			for (int i = 0; (start_x = (max_y - min_y + 1.0) / sqrt(3.0) + 2 * max_x - min_x - i*parallel_line_spacing) >= min_x; end_x = max_x - i*parallel_line_spacing, i++)
			{
				//end_x = end_x - 2.0*parallel_line_spacing / sqrt(3.0);
				//end_x = end_x - parallel_line_spacing;
				Path line;
				line << IntPoint(start_x*scale, start_y*scale)
					<< IntPoint(end_x*scale, end_y*scale);
				lines << line;
			}
			parallel_lines_.push_back(lines);
		}

		
		//if (((i + 2)<n_layers) && ((i + 3)%3 == 0)) //120 degree parallel hatch lines  
		if ((i + 2)<n_layers)
		{
			TriMesh::Contours layer_contours = slice_of_mesh_[i + 2];
			GetHexagonalSubarea get_mesh_hexagonal_subarea_3;
			get_mesh_hexagonal_subarea_3.getMaxAndMinXYofLayer(layer_contours, min_x, max_x, min_y, max_y);

			double start_x = 0.0,
				start_y = min_y - 1.0,
				end_x = max_x,
				end_y = max_y + (max_x - min_x)*sqrt(3.0);
			Paths lines;
			//for (int i = 0; (start_x = min_x - (max_y - min_y + 1.0) / sqrt(3.0) + i*2.0*parallel_line_spacing / sqrt(3.0)) <= max_x;i++)
			for (int i = 0; (start_x = min_x - (max_y - min_y + 1.0) / sqrt(3.0) + i*parallel_line_spacing) <= max_x; end_x = max_x + i* parallel_line_spacing, i++)
			{
				//end_x = end_x + 2.0*parallel_line_spacing / sqrt(3.0);
				//end_x = end_x + parallel_line_spacing ;
				Path line;
				line << IntPoint(start_x*scale, start_y*scale)
					<< IntPoint(end_x*scale, end_y*scale);
				lines << line;
			}
			parallel_lines_.push_back(lines);
		}
	}
}
