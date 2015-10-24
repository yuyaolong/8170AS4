//
//  SpringMesh.hpp
//  assignment2
//
//  Created by yuyaolong on 15/10/22.
//  Copyright © 2015年 yuyaolong. All rights reserved.
//

#ifndef __SPRINGMESH_H__
#define __SPRINGMESH_H__

#include <vector>
#include "Vector.h"
#include "Strut.h"
#include "Face.h"
#include "Point.h"

class SpringMesh {
public:
    std::vector<Strut> struts;
    std::vector<Face> faces;
    std::vector<Point> points;
    SpringMesh();
    
    void show();
    
};




#endif
