#include <vector>

#include "avb_trace.h"
#include "gtest/gtest.h"

class TraceTest : public ::testing::Test {
 protected:
  void SetUp() override {}
  void TearDown() override {}
  uint16_t convertToU16(const uint8_t& index) {
    return (stream[index]) + (((uint16_t)stream[index + 1]) << 8);
  }
  uint32_t convertToU32(const uint8_t& index) {
    return (convertToU16(index)) + (((uint32_t)convertToU16(index + 2)) << 16);
  }
  uint64_t convertToU64(const uint8_t& index) {
    return (convertToU32(index)) + (((uint64_t)convertToU32(index + 4)) << 32);
  }
  std::string convertToString(const uint8_t& index, const uint8_t& length) {
    std::string output;
    output.clear();
    for (int i = 0; i < length; i++) {
      output.push_back(stream[index + i]);
    }
    return output;
  }
  std::vector<uint8_t> stream;
  const uint8_t fid_index = 0;
  const uint8_t act_index = 1;
  const uint8_t len_index = 3;
  const uint8_t first_msg_index = 5;
};

#if 1  // register
TEST_F(TraceTest, CreateRegisterCommand_content_OK) {
  avb_trace trace_command;
  Version version = {1, 2, 3};
  uint16_t act = 0x1234;
  uint16_t length = 3;
  trace_command.register_command_serialize(act, version, stream);
  EXPECT_EQ((uint8_t)FID::FID_REGISTER, stream[fid_index]);
  EXPECT_EQ(act, convertToU16(act_index));
  EXPECT_EQ(length, stream[len_index]);
  EXPECT_EQ(version.versionMajor, stream[first_msg_index]);
  EXPECT_EQ(version.versionMinor, stream[first_msg_index + 1]);
  EXPECT_EQ(version.versionMicro, stream[first_msg_index + 2]);
}

TEST_F(TraceTest, CreateRegisterCommand_getFid_OK) {
  avb_trace trace_command;
  Version version = {1, 2, 3};
  uint16_t act = 0x1234;
  trace_command.register_command_serialize(act, version, stream);
  EXPECT_EQ((uint8_t)FID::FID_REGISTER, trace_command.getFid(stream));
}

TEST_F(TraceTest, DeserializeRegisterCommand_deserialize_OK) {
  avb_trace trace_command;
  Version version = {1, 2, 3};
  uint16_t act = 0x1234;
  trace_command.register_command_serialize(act, version, stream);

  Version version_r = {9, 8, 7};
  uint16_t act_r = 0;
  trace_command.register_command_deserialize(stream, act_r, version_r);

  EXPECT_EQ(act, act_r);
  EXPECT_EQ(version.versionMajor, version_r.versionMajor);
  EXPECT_EQ(version.versionMinor, version_r.versionMinor);
  EXPECT_EQ(version.versionMicro, version_r.versionMicro);
}

TEST_F(TraceTest, CreateRegisterResponse_content_OK) {
  avb_trace aud_response;
  Version version = {1, 2, 3};
  uint16_t act = 0x1234;
  uint16_t length = 3;
  aud_response.register_response_serialize(act, version, stream);
  EXPECT_EQ((uint8_t)FID::FID_REGISTER + 1, stream[fid_index]);
  EXPECT_EQ(act, convertToU16(act_index));
  EXPECT_EQ(length, stream[len_index]);
  EXPECT_EQ(version.versionMajor, stream[first_msg_index]);
  EXPECT_EQ(version.versionMinor, stream[first_msg_index + 1]);
  EXPECT_EQ(version.versionMicro, stream[first_msg_index + 2]);
}

TEST_F(TraceTest, CreateRegisterResponse_getFid_OK) {
  avb_trace aud_response;
  Version version = {1, 2, 3};
  uint16_t act = 0x1234;
  aud_response.register_response_serialize(act, version, stream);
  EXPECT_EQ((uint8_t)FID::FID_REGISTER + 1, aud_response.getFid(stream));
}

TEST_F(TraceTest, DeserializeRegisterResponse_deserialize_OK) {
  avb_trace aud_response;
  Version version = {1, 2, 3};
  uint16_t act = 0x1234;
  aud_response.register_response_serialize(act, version, stream);

  Version version_r = {9, 8, 7};
  uint16_t act_r = 0;
  aud_response.register_response_deserialize(stream, act_r, version_r);

  EXPECT_EQ(act, act_r);
  EXPECT_EQ(version.versionMajor, version_r.versionMajor);
  EXPECT_EQ(version.versionMinor, version_r.versionMinor);
  EXPECT_EQ(version.versionMicro, version_r.versionMicro);
}
#endif

#if 1  // setStreamState
TEST_F(TraceTest, CreateSendTraceBroadcast_content_OK) {
  avb_trace trace_command;
  TraceID traceId = 0x1234;
  TraceContext traceContext = 0x9080706050403020;
  std::string traceMessage = "Trace Testmessage";
  uint16_t act = 0xABCD;
  uint16_t length = sizeof(traceId) + sizeof(traceContext) + traceMessage.size() + 2;
  trace_command.sendTrace_broadcast_serialize(act, traceId, traceContext,
                                               traceMessage, stream);
  EXPECT_EQ((uint8_t)FID::FID_SENDTRACE + 1, stream[fid_index]);
  EXPECT_EQ(act, convertToU16(act_index));
  EXPECT_EQ(length, convertToU16(len_index));
  EXPECT_EQ(traceId, convertToU16(first_msg_index));
  EXPECT_EQ(traceContext, convertToU64(first_msg_index + sizeof(traceId)));
  EXPECT_EQ(traceMessage.size(), convertToU16(first_msg_index + sizeof(traceId) + sizeof(traceContext)));
  EXPECT_TRUE(traceMessage == convertToString(first_msg_index + sizeof(traceId) + sizeof(traceContext) + 2, traceMessage.size()));
}

TEST_F(TraceTest, CreateSendTraceBroadcast_getFid_OK) {
  avb_trace trace_command;
  TraceID traceId = 0x1234;
  TraceContext traceContext = 0x9080706050403020;
  std::string traceMessage = "Trace Testmessage";
  uint16_t act = 0xABCD;
  trace_command.sendTrace_broadcast_serialize(act, traceId, traceContext,
                                               traceMessage, stream);
  EXPECT_EQ((uint8_t)FID::FID_SENDTRACE + 1, trace_command.getFid(stream));
}

TEST_F(TraceTest, DeserializeSendTraceBroadcast_deserialize_OK) {
  avb_trace trace_command;
  TraceID traceId = 0x1234;
  TraceContext traceContext = 0x9080706050403020;
  std::string traceMessage = "Trace Testmessage";
  uint16_t act = 0xABCD;
  trace_command.sendTrace_broadcast_serialize(act, traceId, traceContext,
                                               traceMessage, stream);

  TraceID traceId_r = 0;
  TraceContext traceContext_r = 0;
  std::string traceMessage_r = "";
  uint16_t act_r = 0;
  trace_command.sendTrace_broadcast_deserialize(stream, act_r, traceId_r,
                                                 traceContext_r, traceMessage_r);

  EXPECT_EQ(act, act_r);
  EXPECT_EQ(traceId, traceId_r);
  EXPECT_EQ(traceContext, traceContext_r);
  EXPECT_EQ(traceMessage, traceMessage_r);
}
#endif

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}