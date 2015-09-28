#ifndef _DEBUG_H_
#define _DEBUG_H_


class cDebug
{
protected:

	virtual ~cDebug();

   char str[256];
   int mDebugLevel;
   void dbg_printf(int lvl,const char * fmt,...);
   void dbg_dump_buf(int lvl, void * b,int len);


public:
   cDebug();
   virtual void setDebug(int lvl);


};

#endif //Include Guard
