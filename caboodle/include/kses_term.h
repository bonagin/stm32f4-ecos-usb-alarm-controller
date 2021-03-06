#ifndef _TERM_H_
#define _TERM_H_
#include <cyg/kernel/kapi.h>
#include <cyg/hal/hal_arch.h>
#include <cyg/io/io.h>

class cTerm
{
private:
    static cTerm * __instance;
    cyg_uint32 mRxIdx;
    cyg_uint32 mBuffSize;
    cyg_io_handle_t mDevHandle;
    char * mDev;
    char * mRxBuff;
    char * mPrevBuff;
    const char * mPrompt;

    void banner();
    cyg_bool wasArrow();
    void process_command_line();
    void prompt();
    cTerm(char * dev,cyg_uint32 b_size,const char * const prompt_str);

    //VT100
    bool isVT100();

public:
    static void init(char * dev,cyg_uint32 b_size,const char * const prompt_str);

    static cyg_bool run();

    cTerm& operator<<(void *);
    cTerm& operator<<(const char *);
    cTerm& operator<<(int);
    cTerm& operator<<(unsigned int);
    cTerm& operator<<(unsigned char);
    char * format(const char *f,...);
    virtual ~cTerm();
};

#endif  //Include Guard
