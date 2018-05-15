//
//  drw_wobject_ops.h
//  drw
//
//  Created by Andrew Macfarlane on 9/20/17.
//  Copyright © 2017 ruminant. All rights reserved.
//

#ifndef drw_wobject_ops_h
#define drw_wobject_ops_h

#include <wsh/wsh.h>

WPoint drw_wobject_ops_randompointfromobject(WObject*, int* line_i, int* point_i);
WPoint drw_wobject_ops_randompointfromline(WLine*, int* point_i);

#endif /* drw_wobject_ops_h */
