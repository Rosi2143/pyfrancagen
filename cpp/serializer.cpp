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

buffer serializer::dump_container() {return _message;}

void serializer::print_container() {
  cout << "print_container" << endl << "   ";
  cout << "length = " << dec << _message.size() << endl << "   ";

  for (int i : _message) {
    cout << hex << "0x" << i << " ";
  }
  cout << '\n';
}