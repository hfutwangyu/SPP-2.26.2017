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
protected:
    TriMesh mesh_show_;

    bool draw_points_status_;
    bool draw_edges_status_;
    bool draw_faces_status_;
	bool draw_layers_status_;//add for slice layers 3-8-2017
	bool draw_hexagons_status_;//add for hexagonal subarea 3-8-2017
    // compute the bounding box of a mesh
    bool meshBoundingBox(TriMesh::Point &min_coord, TriMesh::Point &max_coord);
};

#endif // MESHEXAMINER_H
