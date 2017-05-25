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
		un_intersected_subsector_hatch_lines.push_back(lines);
	}
}


void GetHexagonHatches::getHexagonHatchesLines(TriMesh &mesh)
{
	for (int i = 0;i!= un_intersected_subsector_hatch_lines.size(); i++)
	{

		Paths lines = un_intersected_subsector_hatch_lines[i];
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
			un_intersected_subsector_hatch_lines.push_back(lines);
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
			un_intersected_subsector_hatch_lines.push_back(lines);
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
			un_intersected_subsector_hatch_lines.push_back(lines);
		}
	}
}


void GetHexagonHatches::get60ParallelLines(TriMesh::Slicing &slice_of_mesh_, int &scale)
{
	for (auto contours_it = slice_of_mesh_.begin(); contours_it != slice_of_mesh_.end(); contours_it++)
	{
		double min_x = 1.0e8, max_x = 1.0e-9, min_y = 1.0e8, max_y = 1.0e-9;
		TriMesh::Contours layer_contours = *contours_it;
		GetHexagonalSubarea get_mesh_hexagonal_subarea_;
		get_mesh_hexagonal_subarea_.getMaxAndMinXYofLayer(layer_contours, min_x, max_x, min_y, max_y);

		double start_x = 0.0,
			start_y = min_y - 1.0,
			end_x = max_x,
			end_y = max_y + (max_x - min_x)*sqrt(3.0);
		Paths lines;
		for (int i = 0; (start_x = min_x - (max_y - min_y + 1.0) / sqrt(3.0) + i*2.0*parallel_line_spacing / sqrt(3.0)) <= max_x; i++)
			//for (int i = 0; (start_x = min_x - (max_y - min_y + 1.0) / sqrt(3.0) + i*parallel_line_spacing) <= max_x; end_x = max_x + i* parallel_line_spacing, i++)
		{
			end_x = max_x + i*2.0*parallel_line_spacing / sqrt(3.0);
			//end_x = end_x + parallel_line_spacing ;
			Path line;
			line << IntPoint(start_x*scale, start_y*scale)
				<< IntPoint(end_x*scale, end_y*scale);
			lines << line;
		}
		_60_parallel_lines_.push_back(lines);
	}

}


void GetHexagonHatches::get120ParallelLines(TriMesh::Slicing &slice_of_mesh_, int &scale)
{
	for (auto contours_it = slice_of_mesh_.begin(); contours_it != slice_of_mesh_.end(); contours_it++)
	{

		double min_x = 1.0e8, max_x = 1.0e-9, min_y = 1.0e8, max_y = 1.0e-9;
		TriMesh::Contours layer_contours = *contours_it;
		GetHexagonalSubarea get_mesh_hexagonal_subarea_;
		get_mesh_hexagonal_subarea_.getMaxAndMinXYofLayer(layer_contours, min_x, max_x, min_y, max_y);

		double //start_x = (max_y-min_y+1.0)/sqrt(3.0)+2*max_x-min_x,
			start_x = 0.0,
			start_y = min_y - 1.0,
			end_x = max_x,
			end_y = max_y + (max_x - min_x)*sqrt(3.0);
		Paths lines;
		for (int i = 0; (start_x = (max_y - min_y + 1.0) / sqrt(3.0) + 2 * max_x - min_x - 2.0*i*parallel_line_spacing / sqrt(3.0)) >= min_x; i++)
			//for (int i = 0; (start_x = (max_y - min_y + 1.0) / sqrt(3.0) + 2 * max_x - min_x - i*parallel_line_spacing) >= min_x; end_x = max_x - i*parallel_line_spacing, i++)
		{
			end_x = max_x - i*2.0*parallel_line_spacing / sqrt(3.0);
			//end_x = end_x - parallel_line_spacing;
			Path line;
			line << IntPoint(start_x*scale, start_y*scale)
				<< IntPoint(end_x*scale, end_y*scale);
			lines << line;
		}
		_120_parallel_lines_.push_back(lines);
	}
}

void GetHexagonHatches::get0ParallelLines(TriMesh::Slicing &slice_of_mesh_, int &scale)
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
			vertical_min = min_y;
		Paths lines;
		for (int i = 0; (vertical_y = max_y + (max_x - min_x)*sqrt(3.0) - i*parallel_line_spacing) >= vertical_min; i++)
			//for (int i = 0; (vertical_y = max_y + (max_x - min_x)*sqrt(3.0) - i*sqrt(3.0)*parallel_line_spacing/2.0) >= vertical_min; i++)
		{
			Path line;
			line << IntPoint(horizontal_min*scale, vertical_y*scale)
				<< IntPoint(horizontal_max*scale, vertical_y*scale);
			lines << line;
		}
		_0_parallel_lines_.push_back(lines);
	}
}

void GetHexagonHatches::getHexagonHatchesLinesWithTriangularParallelLinesBetween3Columns(TriMesh &mesh,std::vector<std::vector<int>> &hexagon_column_order_)
//5.12.2017
{
	if ((_0_parallel_lines_.size()==_60_parallel_lines_.size())&&
		(_0_parallel_lines_.size() == _120_parallel_lines_.size()))
	{
		for (int i = 0; i < _0_parallel_lines_.size();i++)
		{
			std::vector<int> one_layer_hexagon_column_order_=hexagon_column_order_[i];
			std::vector<Paths> layer_solution;

			for (int j = 0; j < mesh.mesh_hexagoned_hexagons_int_paths_[i].size(); j++)
			{
				int hexagon_column_order_ = one_layer_hexagon_column_order_[j];

				Paths ps_ = mesh.mesh_hexagoned_hexagons_int_paths_[i][j];
				Clipper c;
				PolyTree solution;
				if (hexagon_column_order_%3==0)
				{
					Paths lines = _120_parallel_lines_[i];
					c.AddPaths(lines, ptSubject, false);
					c.AddPaths(ps_, ptClip, true);
					c.Execute(ctIntersection, solution, pftEvenOdd, pftEvenOdd);

					Paths ps;
					OpenPathsFromPolyTree(solution, ps);
					if (ps.size() != 0)
					{
						layer_solution.push_back(ps);
					}
				} 
				else if (hexagon_column_order_%3==2)
				{
					Paths lines = _0_parallel_lines_[i];
					c.AddPaths(lines, ptSubject, false);
					c.AddPaths(ps_, ptClip, true);
					c.Execute(ctIntersection, solution, pftEvenOdd, pftEvenOdd);

					Paths ps;
					OpenPathsFromPolyTree(solution, ps);
					if (ps.size() != 0)
					{
						layer_solution.push_back(ps);
					}
				}
				else if (hexagon_column_order_%3==1)
				{
					Paths lines = _60_parallel_lines_[i];
					c.AddPaths(lines, ptSubject, false);
					c.AddPaths(ps_, ptClip, true);
					c.Execute(ctIntersection, solution, pftEvenOdd, pftEvenOdd);

					Paths ps;
					OpenPathsFromPolyTree(solution, ps);
					if (ps.size() != 0)
					{
						layer_solution.push_back(ps);
					}
				}
	
			}
			mesh.mesh_hexagon_hatches_int_.push_back(layer_solution);
		}
	} 
	else
	{
		exit(1);
	}
}


void GetHexagonHatches::get0ParallelLinesForTriangularHatches(TriMesh::Slicing &slice_of_mesh_, int &scale)
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
			vertical_min = min_y;
		Paths lines;
		for (int i = 0; (vertical_y = max_y + (max_x - min_x)*sqrt(3.0) - i*1.5*parallel_line_spacing) >= vertical_min; i++)
			//for (int i = 0; (vertical_y = max_y + (max_x - min_x)*sqrt(3.0) - i*sqrt(3.0)*parallel_line_spacing/2.0) >= vertical_min; i++)
		{
			Path line;
			line << IntPoint(horizontal_min*scale, vertical_y*scale)
				<< IntPoint(horizontal_max*scale, vertical_y*scale);
			lines << line;
		}
		_0_parallel_lines_for_triangular_hatches.push_back(lines);
	}
}


void GetHexagonHatches::get60ParallelLinesForTriangularHatches(TriMesh::Slicing &slice_of_mesh_, int &scale)
{
	for (auto contours_it = slice_of_mesh_.begin(); contours_it != slice_of_mesh_.end(); contours_it++)
	{
		double min_x = 1.0e8, max_x = 1.0e-9, min_y = 1.0e8, max_y = 1.0e-9;
		TriMesh::Contours layer_contours = *contours_it;
		GetHexagonalSubarea get_mesh_hexagonal_subarea_;
		get_mesh_hexagonal_subarea_.getMaxAndMinXYofLayer(layer_contours, min_x, max_x, min_y, max_y);

		double start_x = 0.0,
			start_y = min_y - 1.0,
			end_x = max_x,
			end_y = max_y + (max_x - min_x)*sqrt(3.0);
		Paths lines;
		for (int i = 0; (start_x = min_x - (max_y - min_y + 1.0) / sqrt(3.0) + i*2.0*1.5*parallel_line_spacing / sqrt(3.0)) <= max_x; i++)
			//for (int i = 0; (start_x = min_x - (max_y - min_y + 1.0) / sqrt(3.0) + i*parallel_line_spacing) <= max_x; end_x = max_x + i* parallel_line_spacing, i++)
		{
			end_x = max_x + i*2.0*1.5*parallel_line_spacing / sqrt(3.0);
			//end_x = end_x + parallel_line_spacing ;
			Path line;
			line << IntPoint(start_x*scale, start_y*scale)
				<< IntPoint(end_x*scale, end_y*scale);
			lines << line;
		}
		_60_parallel_lines_for_triangular_hatches.push_back(lines);
	}
}


void GetHexagonHatches::get120ParallelLinesForTriangularHatches(TriMesh::Slicing &slice_of_mesh_, int &scale)
{
	for (auto contours_it = slice_of_mesh_.begin(); contours_it != slice_of_mesh_.end(); contours_it++)
	{

		double min_x = 1.0e8, max_x = 1.0e-9, min_y = 1.0e8, max_y = 1.0e-9;
		TriMesh::Contours layer_contours = *contours_it;
		GetHexagonalSubarea get_mesh_hexagonal_subarea_;
		get_mesh_hexagonal_subarea_.getMaxAndMinXYofLayer(layer_contours, min_x, max_x, min_y, max_y);

		double //start_x = (max_y-min_y+1.0)/sqrt(3.0)+2*max_x-min_x,
			start_x = 0.0,
			start_y = min_y - 1.0,
			end_x = max_x,
			end_y = max_y + (max_x - min_x)*sqrt(3.0);
		Paths lines;
		for (int i = 0; (start_x = (max_y - min_y + 1.0) / sqrt(3.0) + 2 * max_x - min_x - 2.0*i*1.5*parallel_line_spacing / sqrt(3.0)) >= min_x; i++)
			//for (int i = 0; (start_x = (max_y - min_y + 1.0) / sqrt(3.0) + 2 * max_x - min_x - i*parallel_line_spacing) >= min_x; end_x = max_x - i*parallel_line_spacing, i++)
		{
			end_x = max_x - i*2.0*1.5*parallel_line_spacing / sqrt(3.0);
			//end_x = end_x - parallel_line_spacing;
			Path line;
			line << IntPoint(start_x*scale, start_y*scale)
				<< IntPoint(end_x*scale, end_y*scale);
			lines << line;
		}
		_120_parallel_lines_for_triangular_hatches.push_back(lines);
	}
}


void GetHexagonHatches::getHexagoHatchesWithTriangularHatches(TriMesh &mesh)
{
	if ((_0_parallel_lines_for_triangular_hatches.size() == _60_parallel_lines_for_triangular_hatches.size()) &&
		(_0_parallel_lines_for_triangular_hatches.size() == _120_parallel_lines_for_triangular_hatches.size()))
	{
		for (int i = 0; i < _0_parallel_lines_for_triangular_hatches.size(); i++)
		{
			std::vector<Paths> layer_solution;

			for (int j = 0; j < mesh.mesh_hexagoned_hexagons_int_paths_[i].size(); j++)
			{

				Paths ps_ = mesh.mesh_hexagoned_hexagons_int_paths_[i][j];
				Clipper c_0_,c_60_,c_120;
				PolyTree solution_0_,solution_60_,solution_120_;
		        Paths subsector_solution;

					Paths lines_120_ = _120_parallel_lines_for_triangular_hatches[i];
					c_120.AddPaths(lines_120_, ptSubject, false);
					c_120.AddPaths(ps_, ptClip, true);
					c_120.Execute(ctIntersection, solution_120_, pftEvenOdd, pftEvenOdd);
					Paths ps_120;
					OpenPathsFromPolyTree(solution_120_, ps_120);
					if (ps_120.size() != 0)
					{
						for (int k = 0; k < ps_120.size();k++)
						{
							Path p = ps_120[k];
							subsector_solution.push_back(p);
						}
			
					}
			

					Paths lines_0_ = _0_parallel_lines_for_triangular_hatches[i];
					c_0_.AddPaths(lines_0_, ptSubject, false);
					c_0_.AddPaths(ps_, ptClip, true);
					c_0_.Execute(ctIntersection, solution_0_, pftEvenOdd, pftEvenOdd);
					Paths ps_0_;
					OpenPathsFromPolyTree(solution_0_, ps_0_);
					if (ps_0_.size() != 0)
					{
						for (int k = 0; k < ps_0_.size(); k++)
						{
							Path p = ps_0_[k];
							subsector_solution.push_back(p);
						}
					}


					Paths lines_60_ = _60_parallel_lines_for_triangular_hatches[i];
					c_60_.AddPaths(lines_60_, ptSubject, false);
					c_60_.AddPaths(ps_, ptClip, true);
					c_60_.Execute(ctIntersection, solution_60_, pftEvenOdd, pftEvenOdd);
					Paths ps_60_;
					OpenPathsFromPolyTree(solution_60_, ps_60_);
					if (ps_60_.size() != 0)
					{
						for (int k = 0; k < ps_60_.size(); k++)
						{
							Path p = ps_60_[k];
							subsector_solution.push_back(p);
						}
					}

					layer_solution.push_back(subsector_solution);
			}
			mesh.mesh_hexagon_hatches_int_.push_back(layer_solution);
		}
	}
	else
	{
		exit(1);
	}
}

