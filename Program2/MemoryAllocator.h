//
// Created by trace on 9/29/18.
//

#ifndef LAB03_MEMORYALLOCATOR_H
#define LAB03_MEMORYALLOCATOR_H

#include <vector>
#include <string>
#include <cstdint>
#include <stdexcept>

#include <MMU.h>

class MemoryAllocator {
public:
    MemoryAllocator(uint32_t page_frame_count);
    ~MemoryAllocator(){};
    MemoryAllocator(MemoryAllocator& orig) = delete;
    MemoryAllocator(MemoryAllocator&& orig)= delete;
    MemoryAllocator& operator=(MemoryAllocator& right) = delete;
    MemoryAllocator& operator=(MemoryAllocator&& right)=delete;

    /**
     * pushes the first 'count' frames into the process vector
     *
     * @param count the number of frames to allocate
     * @param page_frames vector of frames allocaetd to the calling process
     * @return true if frames were allocated
     */
    bool AllocatePageFrames(uint32_t count, std::vector<uint32_t> &page_frames);

    /**
     *
     * @param count the number of frames to free
     * @param page_frames vector of frames freed from the calling process
     * @return true if frames were freed
     */
    bool FreePageFrames(uint32_t count, std::vector<uint32_t> &page_frames);

    /**
     *
     * @return string representation of the free list
     */
    std::string get_free_list_string() const;
    /**
     *
     * @param process_id id of the process
     * @return the vector of frames allocated to the process with process_id
     */
    std::vector<uint32_t>& get_process_vector(int process_id);
    /**
     *
     * @return number of free page frames
     */
    uint32_t get_page_frames_free() const { return read_word(page_frames_free);};
private:
    //std::vector<uint8_t> memory;
    mem::MMU *memory;
    std::vector<std::vector<uint32_t>> processes;

    static const uint32_t page_frame_size = 0x2000;
    //offsets to these values in page frame 0, stored immediately after head pointer
    static const uint32_t page_frames_total=sizeof(uint32_t);
    static const uint32_t page_frames_free = 2* sizeof(uint32_t);
    static const uint32_t free_list_head = 3* sizeof(uint32_t);

    /**
     *
     * @param page_number the page to find the address within
     * @param offset the offset desired within page_number
     * @return the absolute memory address of that address
     */
    int memory_index(uint32_t page_number, uint32_t offset) const;
    /**
     *
     * @return total page frames
     */
    uint32_t get_page_frames_total() const {return read_word(page_frames_total);};

    /**
     *
     * @return page frame address of the first frame in the free list
     */
    uint32_t get_free_list_head() const { return read_word(free_list_head);};

    /**
     *
     * @param address address of first of 4 byte sequence
     * @return value of the 4 byte sequence
     */
    uint32_t read_word(uint32_t address) const;

    /**
     *
     * @param page_number 0...n-1 the number of the page frame
     * @param offset byte offset within the page number
     */
    uint32_t read_word(uint32_t page_number, uint32_t offset) const;

    /**
     *
     * @param address start of memory to store 4 bytes
     * @param data bytes to store
     */
    void store_word(uint32_t address, uint32_t data);

    /**
     *
     * @param page_number 0...n-1 the number of the page frame
     * @param offset byte offset within the page number
     * @param data to be written
     */
    void store_word(uint32_t page_number, uint32_t offset, uint32_t data);

    /**
     *
     * @param page_number
     * @return vector address of first byte in page_number
     */
    uint32_t get_page_address(uint32_t page_number);

    /**
     *
     * @param page_frame_count number of page frames
     */
    void initialize_free_list(uint32_t page_frame_count);
};


#endif //LAB03_MEMORYALLOCATOR_H
