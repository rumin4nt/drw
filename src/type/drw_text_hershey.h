//
//  drw_text_hershey.h
//  drw
//
//  Created by vs on 1/11/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#ifndef drw_text_hershey_h
#define drw_text_hershey_h

#ifdef DRW_TEXT_PROVIDER_ENABLE_HERSHEY
void drw_text_hershey_initialize(void);
#endif

void drw_text_hershey_draw(const char* text);

#endif /* drw_text_hershey_h */
