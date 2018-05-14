//
//  d_point.h
//  drw
//
//  Created by vs on 5/1/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#ifndef d_point_h
#define d_point_h

void d_point_2d_to_3d(double x, double y, double* _rx, double* _ry, double* _rz);
void d_point_3d_to_2d(double x, double y, double z, double* rx, double* ry, double* rz );

#endif /* d_point_h */
