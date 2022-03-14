//#include <serialzer.h>
#include <audio.h>

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
  vector<uint8_t> stream_b;

  audio aud_command_send;
  audio aud_command_receive;
  audio aud_response_send;
  audio aud_response_receive;
  audio aud_broadcast_send;
  audio aud_broadcast_receive;

  Version version_c = {.versionMajor = 1, .versionMinor = 2, .versionMicro = 3};
  uint16_t act_c = 0x1234;
  aud_command_send.register_command_serialize(act_c, version_c, stream_c);
  print_container(stream_c, "register command");

  Version version_r = {.versionMajor = 1, .versionMinor = 2, .versionMicro = 3};
  uint16_t act_r = 0;
  if (aud_command_receive.register_command_deserialize(stream_c, act_r,
                                                       version_r))
    cout << "act=" << act_r << ", Majorversion=" << hex
         << +version_r.versionMajor << ", Minorversion=" << hex
         << +version_r.versionMinor << endl;
  else
    cout << "error" << endl;

  version_c = {.versionMajor = 9, .versionMinor = 8, .versionMicro = 7};
  act_c = 0x4321;
  aud_response_send.register_response_serialize(act_c, version_c, stream_r);
  print_container(stream_r, "register response");

  uint16_t act_b = 0xdead;
  BroadcastErrors broadcastError =
      BroadcastErrors::FAILURE_CRF_STREAM_NOT_AVAILABLE;
  string comment("TestMessage 1");
  aud_broadcast_send.errorOccured_broadcast_serialize(act_b, broadcastError,
                                                      comment, stream_b);
  print_container(stream_b, "errorOccured broadcast");
}