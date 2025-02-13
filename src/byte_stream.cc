#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : 
capacity_( capacity ), 
bytes_pushed_(0), 
bytes_popped_(0),
error_(false), 
is_closed_(false), 
is_finished_(false), 
buf_() {}

void Writer::push( string data )
{
  uint64_t num =  data.length();
  if(num > available_capacity())
    num = available_capacity();
  for(uint64_t i = 0;i < num; i++){
    buf_.push_back(data[i]);
  }
  if(num == data.length())
    is_finished_ = true;
  else
    is_finished_ = false;
  
   bytes_pushed_ += num;
}

void Writer::close()
{
  is_closed_ = true;
  is_finished_ = true;
}

bool Writer::is_closed() const
{
  return is_closed_; // Your code here.
}

uint64_t Writer::available_capacity() const
{
  return capacity_ - buf_.length(); // Your code here.
}

uint64_t Writer::bytes_pushed() const
{
  return bytes_pushed_;
}

string_view Reader::peek() const
{
  return string_view{buf_};
}

void Reader::pop( uint64_t len )
{
  buf_ = buf_.substr(len,buf_.length()-len);
  bytes_popped_ += len;
}

bool Reader::is_finished() const
{
  return (is_closed_ && (buf_.size() == 0)); // Your code here.
}

uint64_t Reader::bytes_buffered() const
{
  return buf_.size();
}

uint64_t Reader::bytes_popped() const
{
  return bytes_popped_;
}
