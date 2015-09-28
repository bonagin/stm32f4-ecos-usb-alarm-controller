#include <stdio.h>
#include <cyg/kernel/diag.h>
#include <cyg/kernel/kapi.h>
#include "debug.h"

cDebug::cDebug()
{
   mDebugLevel = 0;
}

void cDebug::setDebug(int lvl)
{
   mDebugLevel = lvl;
}

void cDebug::dbg_printf(int lvl,const char * fmt,...)
{
   if (mDebugLevel >= lvl)
   {
      va_list vl;
      va_start(vl,fmt);
      vsnprintf(str,256,fmt,vl);
      va_end(vl);
      diag_printf(str);
   }
}

void cDebug::dbg_dump_buf(int lvl, void * b,int len)
{
   if (mDebugLevel >= lvl)
   {
      diag_dump_buf(b,len);
   }

}

cDebug::~cDebug()
{

}



