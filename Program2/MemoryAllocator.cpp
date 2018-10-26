//
// Created by trace on 9/29/18.
//

#include "MemoryAllocator.h"
#include <cstring>
#include <sstream>
#include <iomanip>

MemoryAllocator::MemoryAllocator(uint32_t page_frame_count) {
    memory.resize(page_frame_count*page_frame_size);
    processes.resize(4);
    store_word(page_frames_total, page_frame_count);
    store_word(page_frames_free,  page_frame_count -1);
    initialize_free_list(page_frame_count);
}

bool MemoryAllocator::AllocatePageFrames(uint32_t count, std::vector<uint32_t> &page_frames) {
    if(get_page_frames_free()<count){
        //not enouogh page frames to allocate
        return false;
    }
    else {
        uint32_t next_free = get_free_list_head();
        for (int i = 0; i < count; i++) {
            page_frames.push_back(next_free);
            next_free = read_word(next_free);
        }
        store_word(free_list_head, next_free);
        store_word(page_frames_free, get_page_frames_free()-count);
        return true;
    }
}

bool MemoryAllocator::FreePageFrames(uint32_t count, std::vector<uint32_t> &page_frames) {
    if(count>page_frames.size()){
        return false;
    }
    else{
        for(int i=0;i<count;i++){
            uint32_t next_free = page_frames.back();
            page_frames.pop_back();
            uint32_t temp_address = get_free_list_head();
            store_word(free_list_head, next_free);
            store_word(next_free, temp_address);
        }
        store_word(page_frames_free, get_page_frames_free() + count);
        return true;
    }
}



std::string MemoryAllocator::get_free_list_string() const {
    std::stringstream free_list;
    uint32_t next_frame = get_free_list_head();
    while(next_frame!= 0xffffffff){
        free_list<<" "<<std::hex<<next_frame;
        next_frame = read_word(next_frame);
    }
    return std::string(free_list.str());
}

int MemoryAllocator::memory_index(uint32_t page_number, uint32_t offset) const {
    return (page_number/get_page_frames_total()) + offset;
}

uint32_t MemoryAllocator::read_word(uint32_t address) const {
    uint32_t v32;
    memcpy(&v32, &memory[address], sizeof(uint32_t));  // v32 contains the 32-bit value from memory}
}

uint32_t MemoryAllocator::read_word(uint32_t page_number, uint32_t offset) const {
    return read_word(memory_index(page_number, offset));
}

void MemoryAllocator::store_word(uint32_t address, uint32_t data) {
    memcpy(&memory[address], &data, sizeof(uint32_t));
}

void MemoryAllocator::store_word(uint32_t page_number, uint32_t offset, uint32_t data) {
    store_word(memory_index(page_number, offset), data);
}

uint32_t MemoryAllocator::get_page_address(uint32_t page_number) {
    if(page_number>=read_word(page_frames_total)){
        throw std::runtime_error("Requested Page Number greater than total page count\n");
    }
    return page_number * 0x2000;
}

void MemoryAllocator::initialize_free_list(uint32_t page_frame_count) {
    if(page_frame_count == 1){
        store_word(free_list_head, 0xffffffff);
    }
    else if(page_frame_count == 2){
        store_word(free_list_head, get_page_address(1));
        store_word(get_page_address(1), 0xffffffff);
    }
    else{
        store_word(free_list_head, get_page_address(1));
        for (int i = 1; i < page_frame_count-1; i++) {
            store_word(get_page_address(i), get_page_address(i+1));
        }
        store_word(get_page_address(page_frame_count-1), 0xffffffff);
    }
}

std::vector<uint32_t> &MemoryAllocator::get_process_vector(int process_id) {
    if(process_id>3||process_id<0){
        throw std::runtime_error("Process ID out of bounds for current process count\n");
    }
    return processes.at(process_id);
}
