#ifndef SP_MESSAGE_QUEUE_H
#define SP_MESSAGE_QUEUE_H

#pragma once

#include <mutex>
#include <condition_variable>

class SP_Message_Block_Base;

class SP_Message_Queue
{
public:
    enum
    {
        MESSAGE_QUEUE_HIGH_WATER_MARK = 100,

        ACTIVATED = 1,

        DEACTIVATED = 2
    };

    SP_Message_Queue(int hwm = MESSAGE_QUEUE_HIGH_WATER_MARK);
    virtual ~SP_Message_Queue();
    int enqueue(SP_Message_Block_Base *);
    int dequeue(SP_Message_Block_Base *&);
    bool is_full();
    bool is_empty();
    void flush();
    void close();
    void open();
protected:
    int enqueue_i(SP_Message_Block_Base *);
    int dequeue_i(SP_Message_Block_Base *&);
    bool is_full_i();
    bool is_empty_i();
    void flush_i();

private:
    SP_Message_Queue(const SP_Message_Queue &) = delete;
    void operator=(const SP_Message_Queue&) = delete;
private:
    std::mutex mutex_;
    int high_water_mark_;
    std::condition_variable_any not_full_cond_;
    std::condition_variable_any not_empty_cond_;
    int cur_count_;
    SP_Message_Block_Base *head_;
    SP_Message_Block_Base *tail_;
    int state_;
};

#endif

