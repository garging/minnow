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
      std::cout<<"{"<<it->first<<","<<it->second<<"}";
    }  
    std::cout << std::endl;
  };

  print_list();

  // push insert data
  if(pending_data.size() == 0 || first_index < pending_data.begin()->first){
    if(first_index < capacity_end_index){
      pending_data.push_front({first_index,data.substr(0,capacity_end_index-first_index)});
    }
  }
  else{
    auto it = pending_data.begin();
    for(;it != pending_data.end(); it++){
      if(first_index < it->first){
        std::cout<<"pushing before: {" << it->first << "," << it->second << "} with {" << first_index << ", data: " << data << "}" << std::endl;
        
        if(first_index < capacity_end_index){
          pending_data.insert(it, {first_index,data.substr(0,capacity_end_index-first_index)});
        }
      }
    }
    if(it == pending_data.end()) {
      std::cout<<"pushing at the end: {" << first_index << ", data: " << data << "}" << std::endl;
      if(first_index < capacity_end_index){
        pending_data.push_back({first_index,data.substr(0,capacity_end_index-first_index)});
      }
    }
  }

  print_list();

  // process pending bytes
  writer_end_index = output_.writer().bytes_pushed();
  for(auto it = pending_data.begin();it != pending_data.end();){
    if(it->first <= writer_end_index && it->first + it->second.size() > writer_end_index){
      output_.writer().push( it->second.substr(writer_end_index - it->first) );
      std::cout<<"write : " << it->first << "," << it->second << std::endl;
      writer_end_index = output_.writer().bytes_pushed();
    }
    auto tmp = it;
    it++;
    if(tmp->first+tmp->second.size() <= writer_end_index)
      pending_data.erase(tmp);
  }

  if ( is_last_substring )
    output_.writer().close();
}

// How many bytes are stored in the Reassembler itself?
// This function is for testing only; don't add extra state to support it.
uint64_t Reassembler::count_bytes_pending() const
{
  uint64_t count = 0;
  for(auto it=pending_data.begin();it!=pending_data.end();it++)
    count += it->second.size();
  return count;
}
