#ifndef SP_MESSAGE_BLOCK_H
#define SP_MESSAGE_BLOCK_H

#pragma once

class SP_Message_Block_Base
{
public:
    enum Message_Type
    {
        /// Undifferentiated data message
        MB_DATA = 0x01,
        /// Flush your queues
        MB_FLUSH = 0x02,
        /// Stop transmission immediately
        MB_STOP = 0x03,
    };
    SP_Message_Block_Base(const char *data = 0, int idx = 0);
    virtual ~SP_Message_Block_Base();
    void msg_type(unsigned int);
    unsigned int msg_type();
    void next(SP_Message_Block_Base*);
    SP_Message_Block_Base* next();
    void data(const char *);
    char *data();
    void idx(int);
    int idx();

private:
    unsigned int msg_type_;
    SP_Message_Block_Base *next_;
    long msg_no_;
    char *data_;
    int idx_;
};

#endif

