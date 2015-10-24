//
//  SpringMesh.cpp
//  assignment2
//
//  Created by yuyaolong on 15/10/22.
//  Copyright © 2015年 yuyaolong. All rights reserved.
//

#include "SpringMesh.h"


SpringMesh::SpringMesh(){
    
    double ki[9] = {4.92, 2.46, 2.46, 4.92, 4.92, 2.46, 2.46, 4.92, 2.46};
    double di[9] = {5, 2.5, 2.5, 5, 5, 2.5, 2.5, 5, 2.5};
    double li[9] = {2, 2, 2, 2.82, 2, 2.82, 2, 2, 2};
    int vi[9][2] = {{0,1}, {1,2}, {0,3}, {0,4}, {1,4}, {1,5}, {2,5}, {3,4}, {4,5}};
    int fi[9][2] = {{1,-1}, {3,-1}, {-1,0}, {0,1}, {1,2}, {2,3}, {3,-1}, {-1,0}, {-1,2}};
    for (int i=0; i<9; i++) {
        struts.push_back(Strut(ki[i], di[i], li[i], vi[i], fi[i]));
    }
    
    int si[4][3] = {{2,3,7}, {0,4,3}, {4,5,8}, {1,6,5}};
    double vai[4][3] = {{45,45,90}, {90,45,45}, {45,45,90}, {90,45,45}};
    int pi[4][3] = {{0,4,3}, {1,4,0}, {1,5,4}, {2,5,1}};
    for (int i=0; i<4; i++) {
        faces.push_back(Face(si[i], vai[i], pi[i]));
    }
    
    
    double mi[6] = {0.25, 0.5, 0.25, 0.25, 0.5, 0.25};
    Vector3d Xi[6] = {Vector3d(0,0,0), Vector3d(2,0,0), Vector3d(4,0,0),
        Vector3d(0,2,0), Vector3d(2,2,0), Vector3d(4,2,0)};
    Vector3d Vi[6] = {Vector3d(0,0,0), Vector3d(0,0,0), Vector3d(0,0,0),
        Vector3d(0,0,0), Vector3d(0,0,0), Vector3d(0,0,0)};
    Vector3d Fi[6] = {Vector3d(0,0,0), Vector3d(0,0,0), Vector3d(0,0,0),
        Vector3d(0,0,0), Vector3d(0,0,0), Vector3d(0,0,0)};
    for (int i =0; i<6; i++) {
        points.push_back(Point(mi[i], Xi[i], Vi[i], Fi[i]));
    }
    
    
    
   
    
    
}


void SpringMesh::show()
{
    //std::cout<<struts[0].k<<struts[0].d<<struts[0].l<<"\n";
    //std::cout<<struts[0].vertexIndices[1]<<struts[0].faceIndices[1]<<"\n";
    //std::cout<<faces[0].strutIndices[2]<<faces[0].vertexAngles[2]<<"\n";
    
}

