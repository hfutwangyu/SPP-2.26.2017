#ifndef MESHEXAMINER_H
#define MESHEXAMINER_H

#include "glexaminer.h"
#include "mesh.h"

class MeshExaminer : public GLExaminer
{
public:
    MeshExaminer();
    virtual ~MeshExaminer();

    // draw the scene
    virtual void draw();

    void resetMesh(const TriMesh &_mesh, bool _need_normalize);

    void updateMesh(const TriMesh &_mesh);

    void setDrawPointsStatus(bool _val){
        draw_points_status_ = _val;
    }

    void setDrawFacesStatus(bool _val){
        draw_faces_status_ = _val;
    }

    void setDrawEdgesStatus(bool _val){
        draw_edges_status_ = _val;
    }

	void setDrawLayersStatus(bool _val){
		draw_layers_status_ = _val;
	}

	void setDrawHexagonsStatus(bool _val){
		draw_hexagons_status_ = _val;
	}

	void setDrawIntervalsStatus(bool _val){///add 3.14.2017
		draw_intervals_status_ = _val;
	}

	void setDrawParallelHatchessStatus(bool _val){///add 3.15.2017
		draw_parallel_hatches_status_ = _val;
	}

	void setDrawIntervalTrianglesHatchessStatus(bool _val){///add 3.21.2017
		draw_interval_triangles_hatches_status_ = _val;
	}

	void setShowSingleLayerNumber(int num){////////add 3.13.2017
	    show_single_layer_number_ = num;
	}
protected:
    TriMesh mesh_show_;

    bool draw_points_status_;
    bool draw_edges_status_;
    bool draw_faces_status_;
	bool draw_layers_status_;//add for slice layers 3-8-2017
	bool draw_hexagons_status_;//add for hexagonal subarea 3-8-2017
	bool draw_intervals_status_;//sdd for intervals 3.14.2017
	bool draw_parallel_hatches_status_;//add for parallel hatches of hexagonal subareas 3.15.2017
	bool draw_interval_triangles_hatches_status_;//add for triangles hatches of interval 3.21.2017
    // compute the bounding box of a mesh
	int show_single_layer_number_;///add 3.13.2017
    bool meshBoundingBox(TriMesh::Point &min_coord, TriMesh::Point &max_coord);
};

#endif // MESHEXAMINER_H
