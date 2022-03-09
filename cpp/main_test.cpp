//#include <serialzer.h>
#include <test.h>

#include <iostream>

using namespace std;

void print_container(const buffer& buf, const string& name) {
  cout << "print_container for " << name << endl << "   ";
  cout << "length = " << dec << buf.size() << endl << "   ";

  for (int i : buf) {
    cout << hex << "0x" << i << " ";
  }
  cout << '\n';
}

int main() {
  vector<uint8_t> stream_c;
  vector<uint8_t> stream_r;

  test test_command_send;
  test test_command_receive;
  test test_response_send;
  test test_response_receive;

  uint8_t MajorVersion_c = 1;
  uint8_t MinorVersion_c = 2;
  uint16_t act_c = 0x1234;
  test_command_send.register_command_serialize(act_c, MajorVersion_c,
                                              MinorVersion_c, stream_c);
  print_container(stream_c, "register command");

  uint8_t MajorVersion_r = 0;
  uint8_t MinorVersion_r = 0;
  uint16_t act_r = 0;
  if (test_command_receive.register_command_deserialize(
          stream_c, act_r, MajorVersion_r, MinorVersion_r))
    cout << "act=" << act_r << ", Majorversion=" << hex << +MajorVersion_r
         << ", Minorversion=" << hex << +MinorVersion_r << endl;
  else
    cout << "error" << endl;

  MajorVersion_c = 0x0F;
  MinorVersion_c = 0x0E;
  act_c = 0x4321;
  test_command_send.register_response_serialize(act_c, MajorVersion_c,
                                               MinorVersion_c, stream_r);
  print_container(stream_r, "register response");

  MajorVersion_r = 0;
  MinorVersion_r = 0;
  act_r = 0;
  // enable for error tests
  // stream_r[0] = 0x20;
  if (test_command_receive.register_response_deserialize(
          stream_r, act_r, MajorVersion_r, MinorVersion_r))
    cout << "act=" << act_r << ", Majorversion=" << hex << +MajorVersion_r
         << ", Minorversion=" << hex << +MinorVersion_r << endl;
  else
    cout << "error" << endl;
}