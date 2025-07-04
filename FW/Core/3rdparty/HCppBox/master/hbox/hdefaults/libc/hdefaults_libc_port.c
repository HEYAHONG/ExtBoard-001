/***************************************************************
 * Name:      hdefaults_libc_port.c
 * Purpose:   实现hdefaults_libc_port接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-02
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults_libc_port.h"

#include "wrapper/hputchar.c"
#include "wrapper/hgetchar.c"
#include "wrapper/hgetenv.c"
#include "wrapper/hmalloc.c"
#include "wrapper/hfree.c"
#include "wrapper/hcalloc.c"
#include "wrapper/hrealloc.c"
#include "wrapper/habort.c"
#include "wrapper/hexit.c"
#include "wrapper/hfputs.c"
#include "wrapper/hvfprintf.c"
#include "wrapper/hputs.c"
#include "wrapper/hferror.c"
#include "wrapper/hfread.c"
#include "wrapper/hfwrite.c"

