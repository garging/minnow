#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ), error_(false), is_closed_(false), is_finished_(false) {}

void Writer::push( string data )
{
  (void)data; // Your code here.
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
  return capacity_; // Your code here.
}

uint64_t Writer::bytes_pushed() const
{
  return {}; // Your code here.
}

string_view Reader::peek() const
{
  return {}; // Your code here.
}

void Reader::pop( uint64_t len )
{
  (void)len; // Your code here.
}

bool Reader::is_finished() const
{
  return is_finished_; // Your code here.
}

uint64_t Reader::bytes_buffered() const
{
  return {}; // Your code here.
}

uint64_t Reader::bytes_popped() const
{
  return {}; // Your code here.
}
