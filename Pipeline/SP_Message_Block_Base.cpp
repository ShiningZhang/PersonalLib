#include "SP_Message_Block_Base.h"
#include "Global_Macros.h"

SP_Message_Block_Base::SP_Message_Block_Base(const char *data, int idx)
    :msg_type_(Message_Type::MB_DATA),
    next_(0),
    msg_no_(0),
    idx_(idx)
{
    data_ = const_cast <char *>(data);
}

SP_Message_Block_Base::~SP_Message_Block_Base()
{
}

void SP_Message_Block_Base::msg_type(unsigned int msg_type)
{
    this->msg_type_ = msg_type;
}

unsigned int SP_Message_Block_Base::msg_type()
{
    return this->msg_type_;
}

void SP_Message_Block_Base::next(SP_Message_Block_Base* next)
{
    this->next_ = next;
}

SP_Message_Block_Base* SP_Message_Block_Base::next()
{
    return this->next_;
}

void SP_Message_Block_Base::data(const char *data)
{
    data_ = const_cast <char *>(data);
}

char *SP_Message_Block_Base::data()
{
    return this->data_;
}

void SP_Message_Block_Base::idx(int idx)
{
    this->idx_ = idx;
}

int SP_Message_Block_Base::idx()
{
    return this->idx_;
}
