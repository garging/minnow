#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity )
  : capacity_( capacity ), bytes_pushed_( 0 ), bytes_popped_( 0 ), is_closed_( false ), buf_()
{
  buf_.reserve( capacity );
}

void Writer::push( string data )
{
  if ( is_closed() )
    return;
  uint64_t num = min( data.length(), available_capacity() );
  buf_ += data.substr( 0, num );
  bytes_pushed_ += num;
}

void Writer::close()
{
  is_closed_ = true;
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
  return string_view( buf_.data(), min( static_cast<uint64_t>( 1024 ), bytes_buffered() ) );
  // return string_view{buf_};
}

void Reader::pop( uint64_t len )
{
  buf_.erase( buf_.begin(), buf_.begin() + len );
  bytes_popped_ += len;
}

bool Reader::is_finished() const
{
  return ( is_closed_ && ( buf_.size() == 0 ) );
}

uint64_t Reader::bytes_buffered() const
{
  return buf_.size();
}

uint64_t Reader::bytes_popped() const
{
  return bytes_popped_;
}
