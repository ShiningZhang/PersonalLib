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
    SP_Message_Block_Base();
    virtual ~SP_Message_Block_Base();
    void msg_type(unsigned int);
    unsigned int msg_type();
    void delete_data(bool);
    bool delete_data();
    void next(SP_Message_Block_Base*);
    SP_Message_Block_Base* next();

private:
    unsigned int msg_type_;
    bool delete_data_;
    SP_Message_Block_Base *next_;
    long msg_no_;
};

#endif

