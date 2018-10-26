/*
 * Process implementation 
 * 
 * File:   Process.cpp
 * Author: Mike Goss <mikegoss@cs.du.edu>
 * COMP3361 Fall 2018 Lab 2 Sample Solution
 */

#include "Process.h"
#include <MMU.h>

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <sstream>

using std::cin;
using std::cout;
using std::cerr;
using std::getline;
using std::istringstream;
using std::string;
using std::vector;

Process::Process(string file_name_) 
: file_name(file_name_), line_number(0) {
  // Open the trace file.  Abort program if can't open.
  trace.open(file_name, std::ios_base::in);
  if (!trace.is_open()) {
    cerr << "ERROR: failed to open trace file: " << file_name << "\n";
    exit(2);
  }
}

Process::~Process() {
  trace.close();
}

void Process::Run(void) {
  // Read and process commands
  string line;                // text line read
  string cmd;                 // command from line
  vector<uint32_t> cmdArgs;   // arguments from line
  
  // Select the command to execute
  while (ParseCommand(line, cmd, cmdArgs)) {
    if (cmd == "mem" ) {
      CmdMem(line, cmd, cmdArgs);    // allocate memory
    } else if (cmd == "cmp") {
      CmdCmp(line, cmd, cmdArgs);  // get and compare multiple bytes
    } else if (cmd == "sto") {
      CmdSto(line, cmd, cmdArgs);      // put bytes
    } else if (cmd == "rep") {
      CmdRep(line, cmd, cmdArgs);     // fill bytes with value
    } else if (cmd == "cpy") {
      CmdCpy(line, cmd, cmdArgs);     // copy bytes to dest from source
    } else if (cmd == "dmp") {
      CmdDmp(line, cmd, cmdArgs);     // dump byte values to output
    } else if (cmd == "cwp") {
      CmdCwp(line, cmd, cmdArgs);
    } else if (cmd != "#") {
      cerr << "ERROR: invalid command\n";
      exit(2);
    }
  }
}

bool Process::ParseCommand(
    string &line, string &cmd, vector<uint32_t> &cmdArgs) {
  cmdArgs.clear();
  line.clear();
  
  // Read next line
  if (std::getline(trace, line)) {
    ++line_number;
    cout << std::dec << line_number << ":" << line << "\n";
    
    // No further processing if comment or empty line
    if (line.size() == 0 || line[0] == '#') {
      cmd = "#";
      return true;
    }
    
    // Make a string stream from command line
    istringstream lineStream(line);
    
    // Get address
    uint32_t addr = 0;
    if (!(lineStream >> std::hex >> addr)) {
      if (lineStream.eof()) {
        // Blank line, treat as comment
        cmd = "#";
        return true;
      } else {
        cerr << "ERROR: badly formed address in trace file: "
                << file_name << " at line " << line_number << "\n";
        exit(2);
      }
    }
    cmdArgs.push_back(addr);
    
    // Get command
    if (!(lineStream >> cmd)) {
      cerr << "ERROR: no command name following address in trace file: "
              << file_name << " at line " << line_number << "\n";
      exit(2);
    }
    
    // Get any additional arguments
    uint32_t arg;
    while (lineStream >> std::hex >> arg) {
      cmdArgs.push_back(arg);
    }
    return true;
  } else if (trace.eof()) {
      return false;
  } else {
    cerr << "ERROR: getline failed on trace file: " << file_name 
            << " at line " << line_number << "\n";
    exit(2);
  }
}

void Process::CmdMem(const string &line,
                     const string &cmd,
                     const vector<uint32_t> &cmdArgs) {
  if (cmdArgs.size() == 1) {
    // Allocate the specified memory size
    /***mem.resize(cmdArgs.at(0), 0);***/
      uint32_t bytes = cmdArgs.at(0);
      if(bytes % mem::kPageSize == 0){
          //If the input is divisible by the page size, then just allocte the number of pages
          mem::Addr frame_count = bytes/mem::kPageSize;
          memory = std::make_unique<mem::MMU>(frame_count);
      }else{
          //If not, then add to the next multiple of the page size, then allocate the proper num of pages
          mem::Addr frame_count = (bytes+(mem::kPageSize-(bytes % mem::kPageSize))/mem::kPageSize);
          memory = std::make_unique<mem::MMU>(frame_count);
      }
  } else {
    cerr << "ERROR: badly formatted mem command\n";
    exit(2);
  }
}

void Process::CmdCmp(const string &line,
                     const string &cmd,
                     const vector<uint32_t> &cmdArgs) {
  /***uint32_t addr = cmdArgs.at(0);***/
    mem::Addr vaddress = cmdArgs.at(0);
    
    uint8_t buffer;
  // Compare specified byte values
  for (int i = 1; i < cmdArgs.size(); ++i) {
    /***if(mem.at(addr) != cmdArgs.at(i)) {***/
    memory->get_byte(&buffer, vaddress);
    if(buffer != cmdArgs.at(i)){
      /***cout << "cmp error at address " << std::hex << addr***/
        cout << "cmp error at address " << std::hex << vaddress
              << ", expected " << static_cast<uint32_t>(cmdArgs.at(i))
              /***<< ", actual is " << static_cast<uint32_t>(mem.at(addr)) << "\n";***/
              << ", actual is " << static_cast<uint32_t>(buffer) << "\n";
    }
    /***++addr;***/
    ++vaddress;
  }
}

void Process::CmdSto(const string &line,
                     const string &cmd,
                     const vector<uint32_t> &cmdArgs) {
  // Store multiple bytes starting at specified address
  /***uint32_t addr = cmdArgs.at(0);***/
  mem::Addr vaddress = cmdArgs.at(0);
  uint8_t src;
  for (int i = 1; i < cmdArgs.size(); ++i) {
     /***mem.at(addr++) = cmdArgs.at(i);***/
     src = cmdArgs.at(i);
     memory->put_byte(vaddress, &src);
     ++vaddress;
  }
}

void Process::CmdCpy(const string &line,
                     const string &cmd,
                     const vector<uint32_t> &cmdArgs) {
  if (cmdArgs.size() != 3) {
    cerr << "ERROR: badly formatted cpy command\n";
    exit(2);
  }
  // Copy specified number of bytes to destination from source
/***  auto dst = mem.begin() + cmdArgs.at(2); ***/
/***  auto src = mem.begin() + cmdArgs.at(0); ***/
/***  auto src_end = src + cmdArgs.at(1); ***/
/***  std::copy(src, src_end, dst); ***/
  mem::Addr dst = cmdArgs.at(2);
  mem::Addr src = cmdArgs.at(0);
  uint32_t count = cmdArgs.at(1);
  void *buffer;
  memory->get_bytes(&buffer, src, count);
  memory->put_bytes(dst, count, &buffer);
}

void Process::CmdRep(const string &line,
                     const string &cmd,
                     const vector<uint32_t> &cmdArgs) {
  // Replicate specified value in destination range
  uint8_t value = cmdArgs.at(2);
  uint32_t count = cmdArgs.at(1);
  /***uint32_t addr = cmdArgs.at(0);***/
  mem::Addr vaddress = cmdArgs.at(0);
  for (int i = 0; i < count; ++i) {
     /***mem.at(addr++) = value;***/
      memory->put_byte(vaddress, &value);
      ++vaddress;
  }
}

void Process::CmdDmp(const string &line,
                     const string &cmd,
                     const vector<uint32_t> &cmdArgs) {
  /***uint32_t addr = cmdArgs.at(0);***/
    mem::Addr vaddress = cmdArgs.at(0);
  uint32_t count = cmdArgs.at(1);

  // Output the specified number of bytes starting at the address
  for (int i = 0; i < count; ++i) {
    if ((i % 16) == 0) { // Write new line with address every 16 bytes
      if (i > 0) cout << "\n";  // not before first line
      /***cout << std::hex << std::setw(6) << std::setfill('0') << addr << ":";***/
      cout << std::hex << std::setw(6) << std::setfill('0') << vaddress << ":";
    }
    uint8_t buffer;
    memory->get_byte(&buffer, vaddress);
    cout << " " << std::setfill('0') << std::setw(2)
            /***<< static_cast<uint32_t> (mem.at(addr++));***/
            << static_cast<uint32_t>(buffer);
    ++vaddress;
  }
  cout << "\n";
}

void Process::CmdCwp(const string &line,
                    const string &cmd,
                    const vector<uint32_t> &cmdArgs) {
    mem::Addr vaddress = cmdArgs.at(0);
    uint32_t pages = cmdArgs.at(1);
    uint32_t status = cmdArgs.at(2);
    
    for(int i = vaddress; i < vaddress + pages; i++) {
        if(status == 0){
            //clear writable bit to 0
        } else {
            //set writable bit to 1 
        }
    }
}
