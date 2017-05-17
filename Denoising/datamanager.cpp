#include "datamanager.h"
#include <ctime>
DataManager::DataManager()
{

}

bool DataManager::ImportMeshFromFile(std::string filename)
{
    if(!OpenMesh::IO::read_mesh(mesh_, filename))
        return false;
    else
    {
        original_mesh_ = mesh_;
        noisy_mesh_ = mesh_;
        denoised_mesh_ = mesh_;
        return true;
    }
}

bool DataManager::ExportMeshToFile(std::string filename)
{
    return OpenMesh::IO::write_mesh(mesh_, filename);
}


bool DataManager::ExportCLIToFile(std::string filename, std::string first, std::string second, std::string third, std::string forth)
{
	std::string a = first,
		       b=second,
		c=third,d=forth;
	std::ofstream out(filename, std::ios::out|std::ios::trunc);
	if (out.is_open())
	{
		out << "$$HEADERSTART\n";
		out << "$$ASCII\n";
		out << "$$UNITS/1.0\n";
		out << "$$VERSION/100\n";
		out << "$$LABLE/1,part1\n";
		time_t rawtime;
		struct tm * timeinfo;
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		int day = timeinfo->tm_mday;
		int month = timeinfo->tm_mon;
		int year = timeinfo->tm_year%100;
		if ((month+1)<10)
		{
			out << "$$DATE/" << day<<0<< month + 1 << year << "\n";
		}
		else
		{
			out << "$$DATE/" << day << month + 1 << year << "\n";
		}
		
		int layer_num = mesh_.mesh_layers_.size();
		out << "$$LAYERS/"<<layer_num<<"\n";
		out << "$$HEADEREND\n";

		out << "$$GEOMETRYSTART\n";

		if ((mesh_.mesh_hexagon_hatches_double_.size()==mesh_.mesh_interval_hatches_double_.size())&&
			(mesh_.mesh_hexagon_hatches_double_.size()==mesh_.mesh_oriented_slicing_.size())&&
			(mesh_.mesh_hexagon_hatches_double_.size() == mesh_.mesh_segmented_slicing.size()))
		{
			if ((first == "Subarea Hatches") && 
				(second=="Subarea Contour") &&
				(third=="Interval Hatches") &&
				(forth=="Layer Contour"))
			{
				for (int i = 0; i < mesh_.mesh_hexagon_hatches_double_.size();i++)
				{
					double z = mesh_.mesh_layers_[i];
					out << "$$LAYER/" << z << "\n";

					TriMesh::HatchesForOneLayer hatches_of_a_layer=mesh_.mesh_hexagon_hatches_double_[i];
					TriMesh::SegmentedLayers mesh_segmented_layers = mesh_.mesh_segmented_slicing[i];
					//////////////////////////////////////////////////////////////////////////
					//export each layer's hexagonal subarea's hatches & contour
					if (hatches_of_a_layer.size()<=mesh_segmented_layers.size())
					{
						for (int j = 0; j < hatches_of_a_layer.size();j++)
						{
							TriMesh::HatchesForOneHexagonSubarea hatches_of_a_subarea = hatches_of_a_layer[j];
							out << "$$HATCHES/" << 1 << "," << hatches_of_a_subarea.size();
							for (int k = 0; k < hatches_of_a_subarea.size();k++)
							{
								TriMesh::Segment seg=hatches_of_a_subarea[k];
								TriMesh::Point start_pt = seg[0];
								TriMesh::Point end_pt = seg[1];
								out << "," << start_pt[0] << "," << start_pt[1] << "," << end_pt[0] << "," << end_pt[1];
							}
							out << "\n";

							TriMesh::Subareas sub_hexagon_areas = mesh_segmented_layers[j];
							for (int k = 0; k < sub_hexagon_areas.size(); k++)
							{
								TriMesh::Polylines sub_hexagon_area_contour_ = sub_hexagon_areas[k];
								out << "$$POLYLINE/" << 1 << "," << 1 << "," << (sub_hexagon_areas[k].size() + 1);
								for (int l = 0; l < sub_hexagon_area_contour_.size(); l++)
								{
									TriMesh::Point pt = sub_hexagon_area_contour_[l];
									out << "," << pt[0] << "," << pt[1];
								}
								TriMesh::Point pt = sub_hexagon_area_contour_[0];
								out << "," << pt[0] << "," << pt[1] << "\n";
							}
						}
					} 
					else
					{
						exit(2);
					}

					TriMesh::HatchesForOneLayersInterval interval_hatches_of_a_layer=mesh_.mesh_interval_hatches_double_[i];
					//export one layer's interval hatches
					/*int num_of_interval_hatches = 0;
					for (int j = 0; j < interval_hatches_of_a_layer.size(); j++)
					{
						TriMesh::Segment seg = interval_hatches_of_a_layer[j];
						if (seg.size()>2)
						{
							num_of_interval_hatches += (seg.size() + 1);
						}
						else
						{
							num_of_interval_hatches += 2;
						}
					}
					out << "$$HATCHES/" << 1 << "," << num_of_interval_hatches;*/
					out << "$$HATCHES/" << 1 << "," << interval_hatches_of_a_layer.size();
					
					for (int j = 0; j < interval_hatches_of_a_layer.size();j++)
					{
						TriMesh::Segment seg = interval_hatches_of_a_layer[j];
						TriMesh::Point start_pt = seg[0];
						TriMesh::Point end_pt = seg[1];
						out << "," << start_pt[0] << "," << start_pt[1] << "," << end_pt[0] << "," << end_pt[1];

			/*			TriMesh::Segment seg = interval_hatches_of_a_layer[j];
						if (seg.size()>2)
						{
							for (int k = 0; k < seg.size(); k++)
							{
								TriMesh::Point pt = seg[k];
								out << "," << pt[0] << "," << pt[1];
							}
							TriMesh::Point pt = seg[0];
							out << "," << pt[0] << "," << pt[1];
						} 
						else
						{
							TriMesh::Segment seg = interval_hatches_of_a_layer[j];
						    TriMesh::Point start_pt = seg[0];
						    TriMesh::Point end_pt = seg[1];
						    out << "," << start_pt[0] << "," << start_pt[1] << "," << end_pt[0] << "," << end_pt[1];
						}*/
				
					}
					out << "\n";

					TriMesh::OrientedContours oriented_layer_contours=mesh_.mesh_oriented_slicing_[i];
					//////////////////////////////////////////////////////////////////////////
					//export layer's contours
					for (int j = 0; j < oriented_layer_contours.size();j++)
					{
						std::pair<int, TriMesh::Polylines>  a_oriented_layer_contour=oriented_layer_contours[j];
                        out << "$$POLYLINE/" << 1 << "," << a_oriented_layer_contour.first << "," << (a_oriented_layer_contour.second.size() + 1);
						for (int k = 0; k < a_oriented_layer_contour.second.size();k++)
						{
							TriMesh::Point pt = a_oriented_layer_contour.second[k];
							out << "," << pt[0] << "," << pt[1];
						}
						TriMesh::Point pt = a_oriented_layer_contour.second[0];
						out << "," << pt[0] << "," << pt[1] << "\n";
					}
				}
			} 
			else if ((first == "Subarea Contour") &&
				(second == "Subarea Hatches") &&
				(third == "Interval Hatches") &&
				(forth == "Layer Contour"))
			{
				for (int i = 0; i < mesh_.mesh_hexagon_hatches_double_.size(); i++)
				{
					double z = mesh_.mesh_layers_[i];
					out << "$$LAYER/" << z << "\n";

					TriMesh::HatchesForOneLayer hatches_of_a_layer = mesh_.mesh_hexagon_hatches_double_[i];
					TriMesh::SegmentedLayers mesh_segmented_layers = mesh_.mesh_segmented_slicing[i];
					//////////////////////////////////////////////////////////////////////////
					//export each layer's hexagonal subarea's hatches & contour
					if (hatches_of_a_layer.size() <= mesh_segmented_layers.size())
					{
						for (int j = 0; j < hatches_of_a_layer.size(); j++)
						{						
							TriMesh::Subareas sub_hexagon_areas = mesh_segmented_layers[j];
							for (int k = 0; k < sub_hexagon_areas.size(); k++)
							{
								TriMesh::Polylines sub_hexagon_area_contour_ = sub_hexagon_areas[k];
								out << "$$POLYLINE/" << 1 << "," << 1 << "," << (sub_hexagon_areas[k].size() + 1);
								for (int l = 0; l < sub_hexagon_area_contour_.size();l++)
								{
									TriMesh::Point pt = sub_hexagon_area_contour_[l];
									out << "," << pt[0] << "," << pt[1];
								}
								TriMesh::Point pt = sub_hexagon_area_contour_[0];
								out << "," << pt[0] << "," << pt[1] << "\n";
							}
							TriMesh::HatchesForOneHexagonSubarea hatches_of_a_subarea = hatches_of_a_layer[j];
							out << "$$HATCHES/" << 1 << "," << hatches_of_a_subarea.size();
							for (int k = 0; k < hatches_of_a_subarea.size(); k++)
							{
								TriMesh::Segment seg = hatches_of_a_subarea[k];
								TriMesh::Point start_pt = seg[0];
								TriMesh::Point end_pt = seg[1];
								out << "," << start_pt[0] << "," << start_pt[1] << "," << end_pt[0] << "," << end_pt[1];
							}
							out << "\n";
						}
					}
					else
					{
						exit(2);
					}

					TriMesh::HatchesForOneLayersInterval interval_hatches_of_a_layer = mesh_.mesh_interval_hatches_double_[i];
					//export one layer's interval hatches
				/*	int num_of_interval_hatches=0;
					for (int j = 0; j < interval_hatches_of_a_layer.size(); j++)
					{
						TriMesh::Segment seg = interval_hatches_of_a_layer[j];
						if (seg.size()>2)
						{
							num_of_interval_hatches += (seg.size() + 1);
						}
						else 
						{
							num_of_interval_hatches += 2;
						}
					}
					out << "$$HATCHES/" << 1 << "," << num_of_interval_hatches;*/
					out << "$$HATCHES/" << 1 << "," << interval_hatches_of_a_layer.size();
					
					for (int j = 0; j < interval_hatches_of_a_layer.size(); j++)
					{
						TriMesh::Segment seg = interval_hatches_of_a_layer[j];
						TriMesh::Point start_pt = seg[0];
						TriMesh::Point end_pt = seg[1];
						out << "," << start_pt[0] << "," << start_pt[1] << "," << end_pt[0] << "," << end_pt[1];
			/*			TriMesh::Segment seg = interval_hatches_of_a_layer[j];
						if (seg.size()>2)
						{
							for (int k = 0; k < seg.size(); k++)
							{
								TriMesh::Point pt = seg[k];
								out << "," << pt[0] << "," << pt[1];
							}
							TriMesh::Point pt = seg[0];
							out << "," << pt[0] << "," << pt[1];
						}
						else
						{
							TriMesh::Segment seg = interval_hatches_of_a_layer[j];
							TriMesh::Point start_pt = seg[0];
							TriMesh::Point end_pt = seg[1];
							out << "," << start_pt[0] << "," << start_pt[1] << "," << end_pt[0] << "," << end_pt[1];
						}*/

					}
					out << "\n";

					TriMesh::OrientedContours oriented_layer_contours = mesh_.mesh_oriented_slicing_[i];
					//////////////////////////////////////////////////////////////////////////
					//export layer's contours
					for (int j = 0; j < oriented_layer_contours.size(); j++)
					{
						std::pair<int, TriMesh::Polylines>  a_oriented_layer_contour = oriented_layer_contours[j];
						out << "$$POLYLINE/" << 1 << "," << a_oriented_layer_contour.first << "," << (a_oriented_layer_contour.second.size() + 1);
						for (int k = 0; k < a_oriented_layer_contour.second.size(); k++)
						{
							TriMesh::Point pt = a_oriented_layer_contour.second[k];
							out << "," << pt[0] << "," << pt[1];
						}
						TriMesh::Point pt = a_oriented_layer_contour.second[0];
						out << "," << pt[0] << "," << pt[1] << "\n";
					}
				}
			}
		} 
		else
		{
			exit(1);
		}

		out << "$$GEOMETRYEND\n";
		
	}
	return true;
}
