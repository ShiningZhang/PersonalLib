#include "SP_Module.h"
#include "SP_Message_Queue.h"
#include "SP_Message_Block_Base.h"
#include "Global_Macros.h"

SP_Module::SP_Module()
    :thr_count_(0),
    terminated_(true),
    next_(0),
    msg_queue_(0)
{
    SP_TRACE("SP_Module::SP_Module\n");

    SP_NEW(msg_queue_, SP_Message_Queue);

}

SP_Module::~SP_Module()
{
    SP_TRACE("SP_Module::~SP_Module\n");

    SP_DES(msg_queue_);
}

void SP_Module::open()
{
    SP_TRACE("SP_Module::open\n");

}

void SP_Module::close()
{
    SP_TRACE("SP_Module::close\n");

    if (msg_queue_ != 0)
    {
        msg_queue_->close();
    }
    this->wait();
}

void SP_Module::activate(int n_threads /* = 1 */)
{
    SP_TRACE("SP_Module::activate\n");

    std::lock_guard<std::mutex> lock(this->mutex_);
    terminated_ = false;
    thr_count_ += n_threads;
    std::lock_guard<std::mutex> thread_lock(this->thread_mutex_);
    for (int i = 0; i < n_threads; ++i)
    {
        thread_pool.emplace_back(std::bind(&SP_Module::svc_run, this));
    }
}

void SP_Module::wait()
{
    SP_TRACE("SP_Module::wait\n");

    std::lock_guard<std::mutex> thread_lock(this->thread_mutex_);
    for (std::thread & th : thread_pool)
    {
        if (th.joinable())
        {
            th.join();
        }
    }
}

void SP_Module::svc()
{
    SP_TRACE("SP_Module::svc\n");

}

void SP_Module::svc_run()
{
    SP_TRACE("SP_Module::svc_run\n");

    svc();
    std::lock_guard<std::mutex> lock(this->mutex_);
    --thr_count_;
    if (thr_count_ == 0)
    {
        terminated_ = true;
    }
}

int SP_Module::put(SP_Message_Block_Base *msg)
{
    SP_TRACE("SP_Module::put\n");

    return msg_queue_->enqueue(msg);
}

int SP_Module::get(SP_Message_Block_Base *&msg)
{
    SP_TRACE("SP_Module::get\n");

    return msg_queue_->dequeue(msg);
}

void SP_Module::next(SP_Module * next)
{
    SP_TRACE("SP_Module::next\n");

    this->next_ = next;
}

SP_Module * SP_Module::next()
{
    SP_TRACE("SP_Module::next\n");

    return this->next_;
}

int SP_Module::put_next(SP_Message_Block_Base *msg)
{
    SP_TRACE("SP_Module::put_next\n");

    return this->next_ == 0 ? -1 : this->next_->put(msg);
}