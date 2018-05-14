//
//  d_wobject_ops.h
//  drw
//
//  Created by Andrew Macfarlane on 9/20/17.
//  Copyright © 2017 ruminant. All rights reserved.
//

#ifndef d_wobject_ops_h
#define d_wobject_ops_h

#include <wsh/wsh.h>

WPoint d_wobject_ops_randompointfromobject(WObject*, int* line_i, int* point_i);
WPoint d_wobject_ops_randompointfromline(WLine*, int* point_i);

#endif /* d_wobject_ops_h */
