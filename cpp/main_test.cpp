//#include <serialzer.h>
#include <avb_test.h>

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

  avb_test test_command_send;
  avb_test test_command_receive;
  avb_test test_response_send;
  avb_test test_response_receive;

  Version version_c = {.versionMajor = 1, .versionMinor = 2, .versionMicro = 3};
  uint16_t act_c = 0x1234;
  test_command_send.register_command_serialize(act_c, version_c, stream_c);
  print_container(stream_c, "register command");

  Version version_r = {.versionMajor = 1, .versionMinor = 2, .versionMicro = 3};
  uint16_t act_r = 0;
  if (test_command_receive.register_command_deserialize(stream_c, act_r,
                                                        version_r))
    cout << "act=" << act_r << ", Majorversion=" << hex
         << +version_r.versionMajor << ", Minorversion=" << hex
         << +version_r.versionMinor << endl;
  else
    cout << "error" << endl;

  version_c = {.versionMajor = 9, .versionMinor = 8, .versionMicro = 7};
  act_c = 0x4321;
  test_response_send.register_response_serialize(act_c, version_c, stream_r);
  print_container(stream_r, "register response");

  version_r = {.versionMajor = 0, .versionMinor = 0, .versionMicro = 0};
  act_r = 0;
  // enable for error tests
  // stream_r[0] = 0x20;
  if (test_response_receive.register_response_deserialize(stream_r, act_r,
                                                         version_r))
    cout << "act=" << act_r << ", Majorversion=" << hex
         << +version_r.versionMajor << ", Minorversion=" << hex
         << +version_r.versionMinor << endl;
  else
    cout << "error" << endl;
}