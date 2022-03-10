// https://codereview.stackexchange.com/questions/20474/stdvector-memory-manipulation-with-serialization-and-deserialization

#include "serializer.h"

#include <algorithm>
#include <iostream>

using namespace std;

serializer::serializer(bool verbose) {
  _verbose = verbose;
  clear();
}

serializer::serializer(buffer message, bool verbose) {
  _verbose = verbose;
  _message = message;
}

serializer::~serializer() { clear(); }

void serializer::clear() { _message.clear(); }

buffer serializer::dump_container() { return _message; }

void serializer::print_container() {
  cout << "print_container" << endl << "   ";
  cout << "length = " << dec << _message.size() << endl << "   ";

  for (int i : _message) {
    cout << hex << "0x" << i << " ";
  }
  cout << '\n';
}

uint16_t serializer::push_back(const buffer& obj) {
  uint16_t size = obj.size();
  serialize(size);
  for (int i = 0; i < size; i++) {
    _message.push_back((uint8_t)obj[i]);
  }
  return size + sizeof(size);
}

uint16_t serializer::push_back(const string& obj) {
  uint16_t size = obj.size();
  serialize(size);
  for (int i = 0; i < size; i++) {
    _message.push_back((uint8_t)obj[i]);
  }
  return size + sizeof(size);
}

string serializer::pop_front(const string& obj, uint16_t& parameterLength) {
  uint16_t parameterSize = 0;
  uint16_t parameterSizeSize = 0;
  string ret;
  parameterSize = pop_front(parameterSize, parameterSizeSize);
  try {
    if (_message.size() < parameterSize) {
      std::cout << "Buffer too small: length=" << _message.size()
                << " -- obj-size=" << object_size(obj) << std::endl;
      throw std::length_error("buffer too small for object");
    }
    ret.clear();
    for (int i = 0; i < parameterSize; i++) {
      ret.append(1, _message[i]);
    }
    _message.erase(_message.begin(), _message.begin() + parameterSize);
  } catch (const std::exception& e) {
    std::cout << e.what() << '\n';
    //  throw e; // copy-initializes a new exception object of type
    //  std::exception
    throw;  // rethrows the exception object of type std::length_error
  }
  parameterLength = parameterSize + parameterSizeSize;
  return ret;
}

std::vector<uint8_t> serializer::pop_front(const std::vector<uint8_t>& obj, uint16_t& parameterLength)
{
  uint16_t parameterSize = 0;
  uint16_t parameterSizeSize = 0;
  vector<uint8_t> ret;
  parameterSize = pop_front(parameterSize, parameterSizeSize);
  try {
    if (_message.size() < parameterSize) {
      std::cout << "Buffer too small: length=" << _message.size()
                << " -- obj-size=" << parameterSize << std::endl;
      throw std::length_error("buffer too small for object");
    }
    ret.clear();
    for (int i = 0; i < parameterSize; i++) {
      ret.push_back(_message[i]);
    }
    _message.erase(_message.begin(), _message.begin() + parameterSize);
  } catch (const std::exception& e) {
    std::cout << e.what() << '\n';
    //  throw e; // copy-initializes a new exception object of type
    //  std::exception
    throw;  // rethrows the exception object of type std::length_error
  }
  parameterLength = parameterSize + parameterSizeSize;
  return ret;
}
