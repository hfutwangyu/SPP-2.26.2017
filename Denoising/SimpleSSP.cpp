#include "SimpleSSP.h"


SimpleSSP::SimpleSSP(DataManager *_data_manager, ParameterSet *_parameter_set)
: MeshDenoisingBase(_data_manager, _parameter_set)
{
	initParameters();
}


SimpleSSP::~SimpleSSP()
{
}


void SimpleSSP::denoise()
{
	TriMesh mesh = data_manager_->getNoisyMesh();


	Slice slice_of_date_manager;
	parameter_set_->getValue(QString("Layer Thickness"), slice_of_date_manager.thickness);
	//////////////////////////////////////////////////////////////////////////
	///////////////////          slice          ////////////////////////////// 	
	slice_of_date_manager.SliceTheModel(mesh);
	mesh.mesh_layer_thickness_ = slice_of_date_manager.thickness;//add for mesh staircase display 8.31.2017
	mesh.mesh_slicing_ = slice_of_date_manager.slicing;//SET THE SLICING DATA OF THE MESH
	getSlicedLayers(mesh, slice_of_date_manager);//get each layer's position 3.22.2017
	//////////////////////////////////////////////////////////////////////////
	///////////////////          hatches          ////////////////////////////// 
	parameter_set_->getValue(QString("Transformation ration between double data and cInt data"), scale);
	transformLayersDataTypeToInteger(mesh.mesh_slicing_);

	parameter_set_->getValue(QString("Parallel Line Spacing of Hexagonal Subareas"), parallel_line_spacing);
	int hatches_style_index_;
	parameter_set_->getStringListIndex(QString("Hatches style"), hatches_style_index_);
	if (hatches_style_index_ == 0)
	{
		// undirectional hatches
		getParallelLines(mesh.mesh_slicing_, scale);
	}
	else if (hatches_style_index_ == 1)
	{
		// triangular parallel hatches between 3 adjacent layers 4.17.2017
		getTriangularParallelLinesBetweenLayers(mesh.mesh_slicing_, scale);
	}
	getHatchesLines();
	transformHatchesFromCIntToDouble(mesh, slice_of_date_manager);
	getLayerContoursOrientation(mesh, mesh.mesh_slicing_);

	data_manager_->setMesh(mesh);
	data_manager_->setDenoisedMesh(mesh);
}

void SimpleSSP::initParameters()
{
	parameter_set_->removeAllParameter();
	parameter_set_->addParameter(QString("Layer Thickness"), 1.1, QString("Layer Thickness ="), QString("The layer thickness of the sliced model."),
		true, 1.0e-9, 1.0e8);
	parameter_set_->addParameter(QString("Transformation ration between double data and cInt data"), 10000000, QString("Scale ="), QString("Double data multiply scale to get cInt data."),
		true, 1, 100000000000);
	parameter_set_->addParameter(QString("Parallel Line Spacing of Hexagonal Subareas"), 0.11, QString("PLS of HS ="), QString("Parallel Line Spacing of Hexagonal Subareas"),
		true, 1.0e-9, 1.0e8);

	QStringList hatches_style;
	hatches_style.push_back(QString("undirectional hatches"));
	hatches_style.push_back(QString("triangular parallel hatches between 3 adjacent layers"));
	parameter_set_->addParameter(QString("Hatches style"), hatches_style, 0, QString("Hatches style"), QString("The style of Hatches."));
}

void SimpleSSP::getSlicedLayers(TriMesh &mesh, Slice &slice_of_date_manager)
{
	for (int i = 0; i < slice_of_date_manager.slicing.size(); i++)
	{
		//double z = (slice_of_date_manager.model_min_z + (i + 1)*(slice_of_date_manager.thickness));
		double z = (slice_of_date_manager.model_min_z + 0.00001 + i*(slice_of_date_manager.thickness));//10.2.2017
		mesh.mesh_layers_.push_back(z);
	}
}

void SimpleSSP::transformLayersDataTypeToInteger(TriMesh::Slicing &slice_of_mesh_)
//////////////change layers data type from double to cInt
{
	layers_integer_.swap(std::vector<Paths>());
	for (auto contours_it = slice_of_mesh_.begin(); contours_it != slice_of_mesh_.end(); contours_it++)
	{
		TriMesh::Contours layer_contours = *contours_it;
		Paths contours_integer;
		for (auto polylines_it = layer_contours.begin(); polylines_it != layer_contours.end(); polylines_it++)
		{
			TriMesh::Polylines polyline_in_the_contours = *polylines_it;
			Path polyline_integer;
			for (auto points_it = polyline_in_the_contours.begin(); points_it != polyline_in_the_contours.end(); points_it++)
			{
				TriMesh::Point pt_double = *points_it;
				polyline_integer << IntPoint(scale*pt_double[0], scale*pt_double[1]);
			}
			contours_integer << polyline_integer;
		}
		layers_integer_.push_back(contours_integer);
	}
}


void SimpleSSP::getParallelLines(TriMesh::Slicing &slice_of_mesh_, int &scale)
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
			line << IntPoint(horizontal_min*scale, vertical_y*scale)
				<< IntPoint(horizontal_max*scale, vertical_y*scale);
			lines << line;
		}
		un_intersected_hatch_lines.push_back(lines);
	}
}


void SimpleSSP::getTriangularParallelLinesBetweenLayers(TriMesh::Slicing &slice_of_mesh_, int &scale)
// // triangular parallel hatches between adjoint 3 layers 4.17.2017
{
	int n_layers = slice_of_mesh_.size();
	for (int i = 0; i < n_layers; i += 3)
	{
		double min_x = 1.0e8, max_x = 1.0e-9, min_y = 1.0e8, max_y = 1.0e-9;


		//if ((i<n_layers)&&((i+1) % 1 == 0))// 0 degree parallel hatch lines 
		if (i < n_layers)
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
			un_intersected_hatch_lines.push_back(lines);
		}


		//if (((i + 1)<n_layers) && ((i + 2) % 2 == 0)) // 60 degree parallel hatch lines 
		if ((i + 1) < n_layers)
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
			un_intersected_hatch_lines.push_back(lines);
		}


		//if (((i + 2)<n_layers) && ((i + 3)%3 == 0)) //120 degree parallel hatch lines  
		if ((i + 2) < n_layers)
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
			un_intersected_hatch_lines.push_back(lines);
		}
	}
}


void SimpleSSP::getHatchesLines()
{
	for (int i = 0; i != un_intersected_hatch_lines.size(); i++)
	{
		Paths lines = un_intersected_hatch_lines[i];
		std::vector<Path> layer_solution;
		Paths layer = layers_integer_[i];

		Clipper c;
		PolyTree solution;
		c.AddPaths(lines, ptSubject, false);
		c.AddPaths(layer, ptClip, true);
		c.Execute(ctIntersection, solution, pftEvenOdd, pftEvenOdd);

		Paths ps;
		OpenPathsFromPolyTree(solution, ps);
		if (ps.size() != 0)
		{
			for (int j = 0; j < ps.size();j++)
			{
				Path p = ps[j];
				layer_solution.push_back(p);
			}	
		}

		hatchesInt.push_back(layer_solution);
	}
}


void SimpleSSP::transformHatchesFromCIntToDouble(TriMesh &mesh, Slice &slice_of_date_manager)
{
	for (int i = 0; i < hatchesInt.size(); i++)
	{
		//double z = (slice_of_date_manager.model_min_z + (i + 1)*(slice_of_date_manager.thickness));
		double z = (slice_of_date_manager.model_min_z + 0.00001 + i*(slice_of_date_manager.thickness));//10.2.2017
		std::vector<Path> p_layer = hatchesInt[i];
		TriMesh::HatchesOfOneLayer hatches_of_a_layer;
		for (int j = 0; j < p_layer.size(); j++)
		{
			Path p = p_layer[j];
			TriMesh::Segment seg;
			for (int n = 0; n < p.size(); n++)
			{
					IntPoint ipt = p[n];
					TriMesh::Point pt;
					pt[0] = (double)(ipt.X) / scale;
					pt[1] = (double)(ipt.Y) / scale;
					pt[2] = z;
					seg.push_back(pt);
			}
			hatches_of_a_layer.push_back(seg);
		}
		mesh.mesh_hatches_.push_back(hatches_of_a_layer);
	}
	hatchesInt.swap(std::vector<std::vector<Path>>());////free the memory 
	//mesh.mesh_hexagon_hatches_int_.resize(1);////free the memory 

}


void SimpleSSP::getLayerContoursOrientation(TriMesh &mesh, TriMesh::Slicing &slice_of_mesh_)
{
	if (slice_of_mesh_.size() == layers_integer_.size())
	{
		for (int i = 0; i < slice_of_mesh_.size(); i++)
		{
			double min_x = 1.0e8, max_x = 1.0e-9, min_y = 1.0e8, max_y = 1.0e-9;
			TriMesh::Contours layer_contours = slice_of_mesh_[i];
			getMaxAndMinXYofLayer(layer_contours, min_x, max_x, min_y, max_y);

			//////////////////////////////////////////////////////////////////////////
			//adjust the orientation of each layer contour
			Path clip;
			clip << IntPoint((min_x - 10.0)*scale, (min_y - 10.0)*scale) << IntPoint((min_x - 10.0)*scale, (min_y - 20.0)*scale)
				<< IntPoint((min_x - 20.0)*scale, (min_y - 20.0)*scale) << IntPoint((min_x - 20.0)*scale, (min_y - 10.0)*scale);
			Paths contours_integer = layers_integer_[i];
			Clipper C;
			Paths solution;
			C.AddPaths(contours_integer, ptSubject, true);
			C.AddPath(clip, ptClip, true);
			C.Execute(ctDifference, solution, pftEvenOdd, pftEvenOdd);

			//////////////////////////////////////////////////////////////////////////
			//get the oriented slicing data 
			TriMesh::OrientedContours oriented_layer_contours;
			for (int j = 0; j < solution.size(); j++)
			{
				std::pair<int, TriMesh::Polylines>  a_oriented_layer_contour;
				Path p = solution[j];
				//get the orientation
				if (Orientation(p))
				{
					a_oriented_layer_contour.first = 1;
				}
				else
				{
					a_oriented_layer_contour.first = 0;
				}
				//get the polylines
				for (int k = 0; k < p.size(); k++)
				{
					IntPoint int_pt = p[k];
					TriMesh::Point mesh_point;
					mesh_point[0] = (double)(int_pt.X) / scale;
					mesh_point[1] = (double)(int_pt.Y) / scale;
					mesh_point[2] = 0.0;
					a_oriented_layer_contour.second.push_back(mesh_point);
				}
				oriented_layer_contours.push_back(a_oriented_layer_contour);
			}
			mesh.mesh_oriented_slicing_.push_back(oriented_layer_contours);
		}
	}
	else exit(1);
}


void SimpleSSP::getMaxAndMinXYofLayer(TriMesh::Contours &layer_contours, double &min_x, double &max_x, double &min_y, double &max_y)
/////////////////get the max & min x&y in one layer 
{

	for (auto polylines_it = layer_contours.begin(); polylines_it != layer_contours.end(); polylines_it++)
	{
		TriMesh::Polylines polyline_in_the_contours = *polylines_it;
		for (auto points_it = polyline_in_the_contours.begin(); points_it != polyline_in_the_contours.end(); points_it++)
		{
			TriMesh::Point point_in_polyline = *points_it;
			if (point_in_polyline[0] <= min_x) min_x = point_in_polyline[0];
			if (point_in_polyline[0] >= max_x) max_x = point_in_polyline[0];

			if (point_in_polyline[1] <= min_y) min_y = point_in_polyline[1];
			if (point_in_polyline[1] >= max_y) max_y = point_in_polyline[1];
		}
	}
}
