//
//  drw_dummy.h
//  drw
//
//  Created by vs on 5/16/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#ifndef drw_dummy_h
#define drw_dummy_h

#ifndef WSH_COMPAT
#include "dummy/wsh/src/geo/wsh_point.h"
#include "dummy/wsh/src/geo/wsh_line.h"
#include "dummy/wsh/src/geo/wsh_object.h"
#include "dummy/wsh/src/geo/wsh_transform.h"
#endif


#ifndef RUMINANT4_PRESENT
#include "dummy/r4/src/core/r_color.h"
#include "dummy/r4/src/geo/r_line.h"
#include "dummy/r4/src/geo/r_object.h"
#include "dummy/r4/src/geo/r_rect.h"
#include "dummy/r4/src/geo/r_gpc.h"
#endif

#ifndef RUMINANT_GRDN_PRESENT
#include "dummy/grdn/src/env/g_transform.h"
#endif

#endif /* drw_dummy_h */
