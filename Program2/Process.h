/*
 * Process - execute memory trace file in the following format:
 *
 * Trace File Format
 * Trace records contain multiple fields, separated by white space (blanks and 
 * tabs). Each line consists of an address, followed by a command name, 
 * followed by optional hexadecimal integer arguments to the command. 
 * The command name is case sensitive (all lower case).
 * 
 * Addresses are all in the range 0 to FFFFFF.
 * 
 * The trace file will contain the following record types. All numerical values 
 * (including counts) are hexadecimal (base 16), without a leading "0x". 
 * Any output should also use hexadecimal format for numeric data except where 
 * otherwise specified.
 * 
 * Memory Size Specification
 * address mem
 * Set amount of memory to address bytes, addressed from 0 to address-1. 
 * The first non-comment line of the file must be a mem command. Subsequent 
 * mem commands change the amount of allocated memory. Contents of 
 * newly-allocated memory should be initialized to 0.
 * 
 * Find Different Bytes
 * addr cmp expected_values
 * Examine bytes starting at addr; expected_values is a list of byte values, 
 * separated by white space. If the actual values of bytes starting at addr are 
 * different from the expected_values, write an error message to standard error 
 * for each different byte with the address, the expected value, and the actual 
 * value (all in hexadecimal). Follow the format shown in the sample output in 
 * the assignment.
 * 
 * Store Bytes
 * addr sto values
 * Store values starting at addr; values is a list of byte values, separated 
 * by white space. 
 * 
 * Replicate Byte Value
 * addr rep count value
 * Store count copies of value starting at addr.
 * 
 * Copy Bytes
 * src_addr cpy count dest_addr
 * Copy count bytes starting at src_addr into count bytes starting at dest_addr. 
 * The source and destination ranges will not overlap.
 * 
 * Dump Bytes
 * addr dmp count
 * Print count bytes to standard output, starting at address addr. Write 
 * 16 bytes per line, with a single space before each value. Each line should 
 * start with the address of the first byte on that line, 6 hexadecimal digits 
 * with leading zeroes, followed by a colon. Print each byte as exactly 2 digits 
 * with a leading 0 for values less than 10 (hex). Separate the bytes with a 
 * single space. For example, to print the 24 bytes starting at fa700:
 * 
 * 0fa700: 00 12 f3 aa 00 00 00 a0 ff 0f e7 37 21 08 6e 00
 * 0fa710: 55 05 9a 9b 9c ba fa f0
 * 
 * Comment
 * # comment text
 * The # character in the first column means the remainder of the line should be 
 * treated as a comment. The command should be echoed to output in the same way 
 * as other commands, but should otherwise be ignored. Lines which are empty or 
 * all blank should also be treated as comments.
 * 
 * File:   Process.h
 * Author: Mike Goss <mikegoss@cs.du.edu>
 * COMP3361 Fall 2018 Lab 2 Sample Solution
 */

#ifndef PROCESS_H
#define PROCESS_H

#include <MMU.h>

#include <fstream>
#include <string>
#include <vector>
#include <memory>
 
class Process {
public:
  /**
   * Constructor - open trace file, initialize processing
   * 
   * @param file_name_ source of trace commands
   */
  Process(std::string file_name_);
  
  /**
   * Destructor - close trace file, clean up processing
   */
  virtual ~Process(void);

  // Other constructors, assignment: prevent copy and move
  Process(const Process &other) = delete;
  Process(Process &&other) = delete;
  Process operator=(const Process &other) = delete;
  Process operator=(Process &&other) = delete;
  
  /**
   * Run - read and process commands from trace file
   * 
   */
  void Run(void);
  
private:
  // Trace file
  std::string file_name;
  std::fstream trace;
  long line_number;

  // Memory contents
  /***std::vector<uint8_t> mem;***/
  std::unique_ptr<mem::MMU> memory;
  
  /**
   * ParseCommand - parse a trace file command.
   *   Aborts program if invalid trace file.
   * 
   * @param line return the original command line
   * @param cmd return the command name
   * @param cmdArgs returns a vector of argument values
   * @return true if command parsed, false if end of file
   */
  bool ParseCommand(
      std::string &line, std::string &cmd, std::vector<uint32_t> &cmdArgs);
  
  /**
   * Command executors. Arguments are the same for each command.
   *   Form of the function is CmdX, where "X' is the command name, capitalized.
   * @param line original text of command line
   * @param cmd command, converted to all lower case
   * @param cmdArgs arguments to command
   */
  void CmdMem(const std::string &line, 
              const std::string &cmd, 
              const std::vector<uint32_t> &cmdArgs);
  void CmdCmp(const std::string &line, 
              const std::string &cmd, 
              const std::vector<uint32_t> &cmdArgs);
  void CmdSto(const std::string &line, 
              const std::string &cmd, 
              const std::vector<uint32_t> &cmdArgs);
  void CmdRep(const std::string &line, 
              const std::string &cmd, 
              const std::vector<uint32_t> &cmdArgs);
  void CmdCpy(const std::string &line, 
              const std::string &cmd, 
              const std::vector<uint32_t> &cmdArgs);
  void CmdDmp(const std::string &line, 
              const std::string &cmd, 
              const std::vector<uint32_t> &cmdArgs);
  void CmdCwp(const std::string &line, 
              const std::string &cmd, 
              const std::vector<uint32_t> &cmdArgs);
};

#endif /* PROCESS_H */

