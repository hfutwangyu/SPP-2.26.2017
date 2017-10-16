#include "ScanPathPlan.h"
//#include <time.h>
#include <MyTimer.h>


ScanPathPlan::ScanPathPlan(DataManager *_data_manager, ParameterSet *_parameter_set)
: MeshDenoisingBase(_data_manager, _parameter_set)
{
	initParameters();
}


ScanPathPlan::~ScanPathPlan()
{
}

void ScanPathPlan::denoise()
{
	TriMesh mesh = data_manager_->getNoisyMesh();


	Slice slice_of_date_manager;
	parameter_set_->getValue(QString("Layer Thickness"), slice_of_date_manager.thickness);

	GetHexagonalSubarea get_mesh_hexagonal_subarea_;
	bool subsectors_staggered_or_not_;
	bool volume_offset_or_not_;
	bool volume_offset_with_between_triangles_or_not_;
	bool offset_contours_or_not_;
	parameter_set_->getValue(QString("Staggered subsectors between layers"), subsectors_staggered_or_not_);
	parameter_set_->getValue(QString("Volume Offset"), volume_offset_or_not_);
	parameter_set_->getValue(QString("Volume Offset and add between triangles"), volume_offset_with_between_triangles_or_not_);
	parameter_set_->getValue(QString("Offset contours"), offset_contours_or_not_);
	parameter_set_->getValue(QString("Side Length of Rounding hexagon"), get_mesh_hexagonal_subarea_.side_length_of_bounding_hexagon);
	parameter_set_->getValue(QString("Side Length of hexagonal subarea"), get_mesh_hexagonal_subarea_.side_length_of_hexagon);
	parameter_set_->getValue(QString("Transformation ration between double data and cInt data"), get_mesh_hexagonal_subarea_.scale);
	parameter_set_->getValue(QString("Parallel Line Spacing of Hexagonal Subareas"), get_mesh_hexagonal_subarea_.parallel_line_spacing);
	parameter_set_->getValue(QString("Offset"), get_mesh_hexagonal_subarea_.offset);


	//////////////////////////////////////////////////////////////////////////
	///////////////////          slice          ////////////////////////////// 
    MyTimer slice_timer;//add for slicing duration 9.4.2017
	double slice_duration;//add for slicing duration 9.4.2017
	slice_timer.Start();
	slice_of_date_manager.SliceTheModel(mesh);
	slice_timer.End();
	slice_duration = (double)slice_timer.costTime;
	std::ofstream Timing("C:\\Users\\zhangzhen\\Desktop\\Timing.txt", std::ios::out | std::ios::trunc);
	Timing << "slice timing:" << slice_duration << " microseconds.\n";

	mesh.mesh_layer_thickness_ = slice_of_date_manager.thickness;//add for mesh staircase display 8.31.2017
	mesh.mesh_slicing_ = slice_of_date_manager.slicing;//SET THE SLICING DATA OF THE MESH
	getSlicedLayers(mesh,slice_of_date_manager);//get each layer's position 3.22.2017


	//////////////////////////////////////////////////////////////////////////
	///////////////////        honeycomb        //////////////////////////////                       
	get_mesh_hexagonal_subarea_.transformLayersDataTypeToInteger(mesh.mesh_slicing_);
	if (volume_offset_or_not_)
	{
		get_mesh_hexagonal_subarea_.getOuterContoursofEachLayer(mesh.mesh_slicing_);//6.22.2017
		//get_mesh_hexagonal_subarea_.volumeOffsetOnlyDealingWithOuterContours(slice_of_date_manager, mesh.mesh_slicing_);//6.22.2017
		get_mesh_hexagonal_subarea_.volumeOffsetDealingWithAllContours(slice_of_date_manager, mesh.mesh_slicing_);//10.2.2017
		if (volume_offset_with_between_triangles_or_not_)
		{
			slice_of_date_manager.sliceTheModelWithThinkingAboutTrianglesBetween2Layers_UnionOneByOne(mesh, get_mesh_hexagonal_subarea_.volume_offset_layers_integer,get_mesh_hexagonal_subarea_.scale);//9.20.2017
			//slice_of_date_manager.sliceTheModelWithThinkingAboutTrianglesBetween2Layers_AllShapesUnionFirst(mesh, get_mesh_hexagonal_subarea_.volume_offset_layers_integer, get_mesh_hexagonal_subarea_.scale);//10.1.2017
		}
		get_mesh_hexagonal_subarea_.transformVolumeOffsetLayersDataTypeToDouble(slice_of_date_manager, get_mesh_hexagonal_subarea_.volume_offset_layers_integer, mesh.mesh_slicing_);

		//get_mesh_hexagonal_subarea_.transformLayersDataTypeToInteger(mesh.mesh_slicing_);
		get_mesh_hexagonal_subarea_.layers_integer_ = get_mesh_hexagonal_subarea_.volume_offset_layers_integer;

		get_mesh_hexagonal_subarea_.volume_offset_layers_integer.swap(std::vector<Paths>());//6.22.2017
	}

	if (subsectors_staggered_or_not_)
	{
		get_mesh_hexagonal_subarea_.getHexagonsStaggeredBetweenLayers(mesh.mesh_slicing_);//get hexagons staggered between layers 4.22.2017
	}
	else
	{
		get_mesh_hexagonal_subarea_.getHexagons(mesh.mesh_slicing_);///////get hexagons in each layer
	}

	if (offset_contours_or_not_)
	{
		get_mesh_hexagonal_subarea_.segmenLayersIntoHexagonalSubareasWithBoundryOffset(mesh, mesh.mesh_slicing_);//get hexagonal subareas with offsetted boundary 5.2.2017
	} 
	else
	{
		get_mesh_hexagonal_subarea_.segmenLayersIntoHexagonalSubareas(mesh, mesh.mesh_slicing_);//get hexagonal subareas with cInt data type
	}
	get_mesh_hexagonal_subarea_.getLayerContoursOrientation(mesh, mesh.mesh_slicing_);

	//get_mesh_hexagonal_subarea_.layers_integer_.swap(std::vector<Paths> ());//free the memory 3.23.2017
	get_mesh_hexagonal_subarea_.hexagons_in_layers_interger_.swap(std::vector<Paths>());//free the memory 3.23.2017



	//////////////////////////////////////////////////////////////////////////
	/////////////           hatches of  honeycomb            /////////////////
	GetHexagonHatches get_hexagon_hatches_;//get parallel line hatches of hexagonal subareas
	int subsetor_hatches_style_index_;
	parameter_set_->getStringListIndex(QString("Subsector hatches style"), subsetor_hatches_style_index_);
	parameter_set_->getValue(QString("Parallel Line Spacing of Hexagonal Subareas"), get_hexagon_hatches_.parallel_line_spacing);
	if (subsetor_hatches_style_index_==0)
	{
		// undirectional hatches
		get_hexagon_hatches_.getParallelLines(mesh.mesh_slicing_, get_mesh_hexagonal_subarea_.scale);
		get_hexagon_hatches_.getHexagonHatchesLines(mesh);
	} 
	else if (subsetor_hatches_style_index_==1)
	{
		// triangular parallel hatches between 3 adjacent layers 4.17.2017
		get_hexagon_hatches_.getTriangularParallelLinesBetweenLayers(mesh.mesh_slicing_, get_mesh_hexagonal_subarea_.scale);
		get_hexagon_hatches_.getHexagonHatchesLines(mesh);
	}
	else if (subsetor_hatches_style_index_ == 2)
	{
		get_hexagon_hatches_.get0ParallelLines(mesh.mesh_slicing_, get_mesh_hexagonal_subarea_.scale);
		get_hexagon_hatches_.get60ParallelLines(mesh.mesh_slicing_, get_mesh_hexagonal_subarea_.scale);
		get_hexagon_hatches_.get120ParallelLines(mesh.mesh_slicing_, get_mesh_hexagonal_subarea_.scale);
		// triangular parallel hatches between 3 adjacent columns 5.12.2017
		get_hexagon_hatches_.getHexagonHatchesLinesWithTriangularParallelLinesBetween3Columns(mesh, get_mesh_hexagonal_subarea_.hexagon_column_order_);
	}
	else if (subsetor_hatches_style_index_ == 3)
	{
		get_hexagon_hatches_.get0ParallelLinesForTriangularHatches(mesh.mesh_slicing_, get_mesh_hexagonal_subarea_.scale);
		get_hexagon_hatches_.get60ParallelLinesForTriangularHatches(mesh.mesh_slicing_, get_mesh_hexagonal_subarea_.scale);
		get_hexagon_hatches_.get120ParallelLinesForTriangularHatches(mesh.mesh_slicing_, get_mesh_hexagonal_subarea_.scale);
		get_hexagon_hatches_.getHexagoHatchesWithTriangularHatches(mesh);
	}
	transformMeshSegmentedSlicingfromCIntToDouble(mesh, slice_of_date_manager, get_mesh_hexagonal_subarea_);//change the data type from CInt to double
	transformHexagonaHatchesFromCIntToDouble(mesh, slice_of_date_manager, get_mesh_hexagonal_subarea_);//change the data type from CInt to double
	get_hexagon_hatches_.un_intersected_subsector_hatch_lines.swap(std::vector<Paths> ());//free the memory 3.23.2017
	get_hexagon_hatches_._0_parallel_lines_.swap(std::vector<Paths>());//free the memory 5.11.2017
	get_hexagon_hatches_._60_parallel_lines_.swap(std::vector<Paths>());//free the memory 5.11.2017
	get_hexagon_hatches_._120_parallel_lines_.swap(std::vector<Paths>());//free the memory 5.11.2017
	get_hexagon_hatches_._0_parallel_lines_for_triangular_hatches.swap(std::vector<Paths>());//free the memory 5.18.2017
	get_hexagon_hatches_._60_parallel_lines_for_triangular_hatches.swap(std::vector<Paths>());//free the memory 5.18.2017
	get_hexagon_hatches_._120_parallel_lines_for_triangular_hatches.swap(std::vector<Paths>());//free the memory 5.18.2017
	


	//////////////////////////////////////////////////////////////////////////
	/////////////     hatches of  honeycomb's intervals     //////////////////
	GetIntervalHatches get_interval_hatches_;
	bool interval_hatches_cross_subsectors_or_not_;
	parameter_set_->getValue(QString("Interval hathces cross then subsectors"), interval_hatches_cross_subsectors_or_not_);
	parameter_set_->getValue(QString("Parallel Line Spacing of Intervals"), get_interval_hatches_.parallel_line_spacing);
	if ((get_mesh_hexagonal_subarea_.side_length_of_bounding_hexagon - get_mesh_hexagonal_subarea_.side_length_of_hexagon)*sqrt(3.0)<(2 * get_hexagon_hatches_.parallel_line_spacing)&&
		(get_mesh_hexagonal_subarea_.side_length_of_bounding_hexagon - get_mesh_hexagonal_subarea_.side_length_of_hexagon)*sqrt(3.0)> get_hexagon_hatches_.parallel_line_spacing)
		//add Y Connection 5.16.2017
	{
		get_interval_hatches_.getY0ParallelLines(mesh.mesh_slicing_, get_mesh_hexagonal_subarea_.scale,get_mesh_hexagonal_subarea_.side_length_of_bounding_hexagon);
		get_interval_hatches_.getY60ParallelLines(mesh.mesh_slicing_, get_mesh_hexagonal_subarea_.scale, get_mesh_hexagonal_subarea_.side_length_of_bounding_hexagon);
		get_interval_hatches_.getY120ParallelLines(mesh.mesh_slicing_, get_mesh_hexagonal_subarea_.scale, get_mesh_hexagonal_subarea_.side_length_of_bounding_hexagon);
		get_interval_hatches_.getIntervalHatchYConnection(mesh,get_mesh_hexagonal_subarea_.layers_integer_ ,get_mesh_hexagonal_subarea_.interior_hexagons_in_layers_interger_);
	} 
	else
	{
		get_interval_hatches_.get0ParallelLines(mesh.mesh_slicing_, get_mesh_hexagonal_subarea_.scale);
		get_interval_hatches_.get60ParallelLines(mesh.mesh_slicing_, get_mesh_hexagonal_subarea_.scale);
		get_interval_hatches_.get120ParallelLines(mesh.mesh_slicing_, get_mesh_hexagonal_subarea_.scale);

		if (interval_hatches_cross_subsectors_or_not_)
		{
			get_interval_hatches_.getIntervalHatchTrianglesCrossedSubsectors(mesh,get_mesh_hexagonal_subarea_.intervals_with_negative_offseted_subsectors_);
		} 
		else
		{	
			get_interval_hatches_.getIntervalHatchTriangles(mesh);
		}
	
	}
	get_interval_hatches_._0_parallel_lines_.swap(std::vector<Paths>());//free the memory 3.23.2017
	get_interval_hatches_._60_parallel_lines_.swap(std::vector<Paths>());//free the memory 3.23.2017
	get_interval_hatches_._120_parallel_lines_.swap(std::vector<Paths>());//free the memory 3.23.2017

	transformIntervalContoursfromCIntToDouble(mesh, slice_of_date_manager, get_mesh_hexagonal_subarea_);
	transformIntervalsHatchesFromCIntToDouble(mesh, slice_of_date_manager, get_mesh_hexagonal_subarea_);//change the data type from CInt to double
	
	//transformMeshSegmentedSlicingfromCIntToDouble(mesh,slice_of_date_manager,get_mesh_hexagonal_subarea_);//change the data type from CInt to double
   //transformHexagonaHatchesFromCIntToDouble(mesh, slice_of_date_manager, get_mesh_hexagonal_subarea_);//change the data type from CInt to double
	//transformIntervalsHatchesFromCIntToDouble(mesh, slice_of_date_manager, get_mesh_hexagonal_subarea_);//change the data type from CInt to double
	data_manager_->setMesh(mesh);
	data_manager_->setDenoisedMesh(mesh);

	Timing.close();//9.45.2017
}

void ScanPathPlan::getPlanPath()
{

}

void ScanPathPlan::initParameters()
{
	parameter_set_->removeAllParameter();
	parameter_set_->addParameter(QString("Layer Thickness"), 1.1, QString("Layer Thickness ="), QString("The layer thickness of the sliced model."),
		true, 1.0e-9, 1.0e8);

	parameter_set_->addParameter(QString("Volume Offset"), false, QString("Volume Offset"), QString("Volume Offset or not."));//6.22.2017
	parameter_set_->addParameter(QString("Volume Offset and add between triangles"), false, QString("Volume Offset and add between triangles"), QString("Volume Offset and add between triangles or not."));

	parameter_set_->addParameter(QString("Offset contours"), false, QString("Offset contours"), QString("Offset contours or not."));
	parameter_set_->addParameter(QString("Interval hathces cross then subsectors"), false, QString("Interval hathces cross then subsectors"), QString("Interval hathces cross then subsectors or not."));
	parameter_set_->addParameter(QString("Staggered subsectors between layers"), false, QString("Staggered subsectors between layers"), QString("Subsectors staggered or not bwetween layers."));
	parameter_set_->addParameter(QString("Side Length of Rounding hexagon"), 2.0, QString("SL of RH ="), QString("The side length of rounding hexagon."),
		true, 1.0e-9, 1.0e8);
	parameter_set_->addParameter(QString("Side Length of hexagonal subarea"), 1.7, QString("SL of HS ="), QString("The side length of hexagonal subarea, less than SL of RH."),
		true, 1.0e-9, 1.0e8);
	parameter_set_->addParameter(QString("Transformation ration between double data and cInt data"), 10000000, QString("Scale ="), QString("Double data multiply scale to get cInt data."),
		true, 1, 100000000000);

	QStringList subsector_hatches_style;
	subsector_hatches_style.push_back(QString("undirectional hatches"));
	subsector_hatches_style.push_back(QString("triangular parallel hatches between 3 adjacent layers"));
	subsector_hatches_style.push_back(QString("triangular parallel hatches between 3 adjacent columns"));//5.12.2017
	subsector_hatches_style.push_back(QString("triangular hatches"));//5.18.2017
	parameter_set_->addParameter(QString("Subsector hatches style"), subsector_hatches_style, 0, QString("Subsector hatches style"), QString("The style of subsector hatches."));

	parameter_set_->addParameter(QString("Parallel Line Spacing of Hexagonal Subareas"), 0.11, QString("PLS of HS ="), QString("Parallel Line Spacing of Hexagonal Subareas"), 
		true, 1.0e-9, 1.0e8);
	parameter_set_->addParameter(QString("Parallel Line Spacing of Intervals"), 0.165, QString("PLS of Intervals ="), QString("Parallel Line Spacing of Intervals"),
		true, 1.0e-9, 1.0e8);
	parameter_set_->addParameter(QString("Offset"), 0.2, QString("Offset ="), QString("Contours' Offset"),
		true, 1.0e-9, 1.0e8);
	parameter_set_->setName(QString("SPP"));
	parameter_set_->setLabel(QString("SPP"));
	parameter_set_->setIntroduction(QString("SPP -- Parameters"));
}

void ScanPathPlan::transformMeshSegmentedSlicingfromCIntToDouble(TriMesh &mesh, Slice &slice_of_date_manager, GetHexagonalSubarea &get_mesh_hexagonal_subarea_)
{
	for (int i = 0; i < mesh.mesh_hexagoned_hexagons_int_paths_.size(); i++)//segment layers into hexagonal subareas
	{
		//double z = (slice_of_date_manager.model_min_z + (i + 1)*(slice_of_date_manager.thickness));
		double z = (slice_of_date_manager.model_min_z + 0.00001+ i *(slice_of_date_manager.thickness));//10.2.2017
		std::vector<Paths> a_layer_subareas_paths_ = mesh.mesh_hexagoned_hexagons_int_paths_[i];
		TriMesh::SegmentedLayers mesh_segmented_layers;
		for (int j = 0; j < a_layer_subareas_paths_.size(); j++)
		{
			Paths a_subarea_paths_ = a_layer_subareas_paths_[j];
			TriMesh::Subareas sub_hexagon_areas;
			for (int k = 0; k < a_subarea_paths_.size(); k++)
			{
				Path a_path = a_subarea_paths_[k];
				TriMesh::Polylines a_subarea_contour_;
				for (int l = 0; l < a_path.size();l++)
				{
					IntPoint int_pt = a_path[l];
					TriMesh::Point mesh_point;
					mesh_point[0] = (double)(int_pt.X) / (get_mesh_hexagonal_subarea_.scale);
					mesh_point[1] = (double)(int_pt.Y) / (get_mesh_hexagonal_subarea_.scale);
					mesh_point[2] = z;

					a_subarea_contour_.push_back(mesh_point);
				}
				sub_hexagon_areas.push_back(a_subarea_contour_);
			}
			mesh_segmented_layers.push_back(sub_hexagon_areas);
		}
		mesh.mesh_segmented_slicing.push_back(mesh_segmented_layers);
	}

	mesh.mesh_hexagoned_hexagons_int_paths_.swap(std::vector<std::vector<Paths>> ());//free the memory 3.23.2017
	//mesh.mesh_hexagoned_hexagons_int_paths_.resize(1);//free the memory 3.23.2017
}


void ScanPathPlan::transformHexagonaHatchesFromCIntToDouble(TriMesh &mesh, Slice &slice_of_date_manager, GetHexagonalSubarea &get_mesh_hexagonal_subarea_)
{
	for (int i = 0; i < mesh.mesh_hexagon_hatches_int_.size();i++)
	{
		//double z = (slice_of_date_manager.model_min_z + (i + 1)*(slice_of_date_manager.thickness));
		double z = (slice_of_date_manager.model_min_z +0.00001 + i*(slice_of_date_manager.thickness));//10.2.2017
		std::vector<Paths> ps_layer = mesh.mesh_hexagon_hatches_int_[i];
		TriMesh::HatchesForOneLayer hatches_of_a_layer;
		for (int j = 0; j < ps_layer.size();j++)
		{
			Paths ps = ps_layer[j];
			TriMesh::HatchesForOneHexagonSubarea hatches_of_a_subarea;
			for (int k = 0; k < ps.size();k++)
			{
				Path P = ps[k];
				TriMesh::Segment seg;
				for (int n = 0; n < P.size();n++)
				{
					IntPoint ipt = P[n];
					TriMesh::Point pt;
					pt[0] = (double)(ipt.X) / (get_mesh_hexagonal_subarea_.scale);
					pt[1] = (double)(ipt.Y) / (get_mesh_hexagonal_subarea_.scale);
					pt[2] = z;	
					seg.push_back(pt);
				}
				hatches_of_a_subarea.push_back(seg);
			}
			hatches_of_a_layer.push_back(hatches_of_a_subarea);
		}
		mesh.mesh_hexagon_hatches_double_.push_back(hatches_of_a_layer);
	}

	mesh.mesh_hexagon_hatches_int_.swap(std::vector<std::vector<Paths>> ());////free the memory 2.23.2017
	//mesh.mesh_hexagon_hatches_int_.resize(1);////free the memory 2.23.2017

}


void ScanPathPlan::transformIntervalsHatchesFromCIntToDouble(TriMesh &mesh, Slice &slice_of_date_manager, GetHexagonalSubarea &get_mesh_hexagonal_subarea_)
{
	for (int i = 0; i < mesh.mesh_interval_hatches_int_.size();i++)
	{
		//double z = (slice_of_date_manager.model_min_z + (i + 1)*(slice_of_date_manager.thickness));
		double z = (slice_of_date_manager.model_min_z + 0.00001 + i*(slice_of_date_manager.thickness));//10.2.2017
		std::vector<Paths> ps_layer = mesh.mesh_interval_hatches_int_[i];
		TriMesh::HatchesForOneLayersInterval interval_hatches_of_a_layer;
		for (int j = 0; j < ps_layer.size();j++)
		{
			Paths ps = ps_layer[j];
			for (int k = 0; k < ps.size();k++)
			{
				Path p = ps[k];
				//TriMesh::Segment seg;
				//for (int n = 0; n < p.size(); n++)
				//{
				//	IntPoint ipt = p[n];
				//	TriMesh::Point pt;
				//	pt[0] = (double)(ipt.X) / (get_mesh_hexagonal_subarea_.scale);
				//	pt[1] = (double)(ipt.Y) / (get_mesh_hexagonal_subarea_.scale);
				//	pt[2] = z;
				//	seg.push_back(pt);
				//}
				if (p.size() > 2)
				{
					for (int k = 0; k < p.size() - 1; k++)
					{
						TriMesh::Segment seg;
						IntPoint int_pt_s = p[k];
						TriMesh::Point mesh_point_s;
						mesh_point_s[0] = (double)(int_pt_s.X) / (get_mesh_hexagonal_subarea_.scale);
						mesh_point_s[1] = (double)(int_pt_s.Y) / (get_mesh_hexagonal_subarea_.scale);
						mesh_point_s[2] = z;

						IntPoint int_pt_e = p[k + 1];
						TriMesh::Point  mesh_point_e;
						seg.push_back(mesh_point_s);
						mesh_point_e[0] = (double)(int_pt_e.X) / (get_mesh_hexagonal_subarea_.scale);
						mesh_point_e[1] = (double)(int_pt_e.Y) / (get_mesh_hexagonal_subarea_.scale);
						mesh_point_e[2] = z;
						seg.push_back(mesh_point_e);

						interval_hatches_of_a_layer.push_back(seg);
					}

					TriMesh::Segment seg;

					IntPoint int_pt_s = p.back();
					TriMesh::Point mesh_point_s;
					mesh_point_s[0] = (double)(int_pt_s.X) / (get_mesh_hexagonal_subarea_.scale);
					mesh_point_s[1] = (double)(int_pt_s.Y) / (get_mesh_hexagonal_subarea_.scale);
					mesh_point_s[2] = z;
                    seg.push_back(mesh_point_s);

					IntPoint int_pt_e = p.front();
					TriMesh::Point  mesh_point_e;
					mesh_point_e[0] = (double)(int_pt_e.X) / (get_mesh_hexagonal_subarea_.scale);
					mesh_point_e[1] = (double)(int_pt_e.Y) / (get_mesh_hexagonal_subarea_.scale);
					mesh_point_e[2] = z;
					seg.push_back(mesh_point_e);

					interval_hatches_of_a_layer.push_back(seg);
				}
				else if (p.size() == 2)
				{
					TriMesh::Segment seg;
					for (int k = 0; k < p.size(); k++)
					{
						IntPoint int_pt = p[k];
						TriMesh::Point mesh_point;
						mesh_point[0] = (double)(int_pt.X) / (get_mesh_hexagonal_subarea_.scale);
						mesh_point[1] = (double)(int_pt.Y) / (get_mesh_hexagonal_subarea_.scale);
						mesh_point[2] = z;
						seg.push_back(mesh_point);
					}
					interval_hatches_of_a_layer.push_back(seg);
				}
				//interval_hatches_of_a_layer.push_back(seg);
			}
		}
		mesh.mesh_interval_hatches_double_.push_back(interval_hatches_of_a_layer);
	}

	mesh.mesh_interval_hatches_int_.swap(std::vector<std::vector<Paths>> ());//////free the memory 2.23.2017
	//mesh.mesh_interval_hatches_int_.resize(1);//////free the memory 2.23.2017
}


void ScanPathPlan::getSlicedLayers(TriMesh &mesh, Slice &slice_of_date_manager)
{
	for (int i = 0; i < slice_of_date_manager.slicing.size(); i++)
	{
		//double z = (slice_of_date_manager.model_min_z + (i + 1)*(slice_of_date_manager.thickness));
		double z = (slice_of_date_manager.model_min_z +0.00001+ i*(slice_of_date_manager.thickness));//10.2.2017
		mesh.mesh_layers_.push_back(z);
	}
}


void ScanPathPlan::transformIntervalContoursfromCIntToDouble(TriMesh &mesh, Slice &slice_of_date_manager, GetHexagonalSubarea &get_mesh_hexagonal_subarea_)
{
	for (int i = 0; i < mesh.mesh_areas_betweent_hexagons_int_paths_.size(); i++)//intervals between hexagonal subareas
	{
		//double z = (slice_of_date_manager.model_min_z + (i + 1)*(slice_of_date_manager.thickness));
		double z = (slice_of_date_manager.model_min_z + 0.00001 + i*(slice_of_date_manager.thickness));//10.2.2017
		Paths ps = mesh.mesh_areas_betweent_hexagons_int_paths_[i];
		TriMesh::BetweenSegmentedLayers mesh_between_segmented_layers;
		for (int j = 0; j < ps.size(); j++)
		{
			Path p = ps[j];
			TriMesh::BetweenSubareas between_hexagon_sunareas_;
			for (int k = 0; k < p.size(); k++)
			{
				IntPoint int_pt = p[k];
				TriMesh::Point mesh_point;
				mesh_point[0] = (double)(int_pt.X) / (get_mesh_hexagonal_subarea_.scale);
				mesh_point[1] = (double)(int_pt.Y) / (get_mesh_hexagonal_subarea_.scale);
				mesh_point[2] = z;
				between_hexagon_sunareas_.push_back(mesh_point);
			}
			mesh_between_segmented_layers.push_back(between_hexagon_sunareas_);
		}
		mesh.mesh_between_segmented_slicing_.push_back(mesh_between_segmented_layers);
	}

	mesh.mesh_areas_betweent_hexagons_int_paths_.swap(std::vector<Paths> ());//free the memory 2.23.2017
	//mesh.mesh_areas_betweent_hexagons_int_paths_.resize(1);//free the memory 2.23.2017
}
