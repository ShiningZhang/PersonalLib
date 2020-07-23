#include <stdio.h>
#include "SP_Stream.h"
#include "Global_Macros.h"

class req
{
};

class m1 : public SP_Module
{
public:
    m1(int threads = 1) : threads_num_(threads){};
    virtual ~m1(){};
    virtual int open()
    {
        activate(threads_num_);
        return 0;
    };
    virtual void svc()
    {
        for (SP_Message_Block_Base *msg = 0; get(msg) != -1;)
        {
            req *r = reinterpret_cast<req *>(msg->data());
            printf("m1:r:%p\n",r);
            put_next(msg);
        }
    };
    virtual int init() { return 0; };

private:
    int threads_num_;
    std::mutex lock_;
};

class m2 : public SP_Module
{
public:
    m2(int threads = 1) : threads_num_(threads){};
    virtual ~m2(){};
    virtual int open()
    {
        activate(threads_num_);
        return 0;
    };
    virtual void svc()
    {
        for (SP_Message_Block_Base *msg = 0; get(msg) != -1;)
        {
            req *r = reinterpret_cast<req *>(msg->data());
            printf("m2:r:%p\n",r);
            put_next(msg);
        }
    };
    virtual int init() { return 0; };

private:
    int threads_num_;
    std::mutex lock_;
};

int main(int argc, char **argv)
{
    SP_Stream *s_instance_stream = new SP_Stream;
    SP_Module *modules[10];
    SP_NEW_RETURN(s_instance_stream, SP_Stream, -1);
    SP_NEW_RETURN(modules[0], m1(1), -1);
    SP_NEW_RETURN(modules[1], m2(8), -1);

    for (int i = 1; i >= 0; --i)
    {
        s_instance_stream->push_module(modules[i]);
    }
    req *data = new req();
    SP_Message_Block_Base *msg = NULL;
    SP_NEW_RETURN(msg, SP_Message_Block_Base((char *)data), -1);
    s_instance_stream->put(msg);
    s_instance_stream->get(msg);
    req *r = reinterpret_cast<req *>(msg->data());
    delete msg;
    printf("r:%p\n", r);
    delete r;
    printf("end\n");
    return 0;
}