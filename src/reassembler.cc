#include "reassembler.hh"
#include "debug.hh"

using namespace std;

void Reassembler::insert( uint64_t first_index, string data, bool is_last_substring )
{
  uint64_t writer_end_index, capacity_end_index;
  writer_end_index = output_.writer().bytes_pushed();
  capacity_end_index = writer_end_index + output_.writer().available_capacity();
  std::cout<<"writer_end_index: "<< writer_end_index << ", capacity_end_index: " << capacity_end_index << std::endl;
  std::cout<<"bytes pushed: "<< output_.writer().bytes_pushed() << ", try to insert: " << first_index << ", data: " << data << ", ";
  auto print_list = [&](){
    std::cout<<"pending_data: ";
    for(auto it = pending_data.begin();it != pending_data.end(); it++){
      std::cout<<"{"<<std::get<0>(*it)<<","<<std::get<1>(*it)<<"}";
    }  
    std::cout << std::endl;
  };

  print_list();

  // push insert data
  if(pending_data.size() == 0 || first_index < std::get<0>(*pending_data.begin())){
    if(first_index < capacity_end_index){
      if(capacity_end_index-first_index < data.size())
        is_last_substring = false;
      pending_data.push_front({first_index,data.substr(0,capacity_end_index-first_index),is_last_substring});
    }
  }
  else{
    auto it = pending_data.begin();
    for(;it != pending_data.end(); it++){
      if(first_index < std::get<0>(*it)){
        std::cout<<"pushing before: {" << std::get<0>(*it) << "," << std::get<1>(*it) << "} with {" << first_index << ", data: " << data << "}" << std::endl;
        
        if(first_index < capacity_end_index){
          if(capacity_end_index-first_index < data.size())
            is_last_substring = false;
          pending_data.insert(it, {first_index,data.substr(0,capacity_end_index-first_index),is_last_substring});
        }
      }
    }
    if(it == pending_data.end()) {
      std::cout<<"pushing at the end: {" << first_index << ", data: " << data << "}" << std::endl;
      if(first_index < capacity_end_index){
        if(capacity_end_index-first_index < data.size())
          is_last_substring = false;
        pending_data.push_back({first_index,data.substr(0,capacity_end_index-first_index),is_last_substring});
      }
    }
  }

  print_list();

  // process pending bytes
  writer_end_index = output_.writer().bytes_pushed();
  for(auto it = pending_data.begin();it != pending_data.end();){
    std::cout<<"writer_end_index: "<<writer_end_index<<", checking: {" << std::get<0>(*it) << ", data: " << std::get<1>(*it) <<", close: " << std::get<2>(*it) << "}" << std::endl;
    if(std::get<0>(*it) <= writer_end_index && std::get<0>(*it) + std::get<1>(*it).size() >= writer_end_index){
      output_.writer().push( std::get<1>(*it).substr(writer_end_index - std::get<0>(*it)) );
      std::cout<<"write : " << std::get<0>(*it) << "," << std::get<1>(*it).substr(writer_end_index - std::get<0>(*it))<<", close: " << std::get<2>(*it) << std::endl;
      writer_end_index = output_.writer().bytes_pushed();
      std::cout<<"writer_end_index changed to: "<<writer_end_index<<std::endl;
      if ( std::get<2>(*it) ){
        std::cout<<"last is true, closing writer"<<std::endl;
        output_.writer().close();
      }
    }
    auto tmp = it;
    it++;
    if(std::get<0>(*tmp)+std::get<1>(*tmp).size() <= writer_end_index){
      std::cout<<"{" << std::get<0>(*tmp) << ", data: " << std::get<1>(*tmp)<<", close: " << std::get<2>(*tmp) << "} removed" << std::endl;
      pending_data.erase(tmp);
    }
  }
}

// How many bytes are stored in the Reassembler itself?
// This function is for testing only; don't add extra state to support it.
uint64_t Reassembler::count_bytes_pending() const
{
  uint64_t count = 0;
  for(auto it=pending_data.begin();it!=pending_data.end();it++)
    count += std::get<1>(*it).size();
  return count;
}
