#include <vector>

#include "audio.h"
#include "gtest/gtest.h"

class AudioTest : public ::testing::Test {
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
    return (convertToU32(index)) + (((uint64_t)convertToU16(index + 4)) << 32);
  }
  std::vector<uint8_t> stream;
  const uint8_t fid_index = 0;
  const uint8_t act_index = 1;
  const uint8_t len_index = 3;
  const uint8_t first_msg_index = 5;
};

// register
TEST_F(AudioTest, CreateRegisterCommand_content_OK) {
  audio aud_command;
  uint8_t MajorVersion = 1;
  uint8_t MinorVersion = 2;
  uint16_t act = 0x1234;
  uint16_t length = 2;
  aud_command.register_command_serialize(act, MajorVersion, MinorVersion,
                                         stream);
  EXPECT_EQ((uint8_t)FID::FID_REGISTER, stream[fid_index]);
  EXPECT_EQ(act, convertToU16(act_index));
  EXPECT_EQ(length, stream[len_index]);
  EXPECT_EQ(MajorVersion, stream[first_msg_index]);
  EXPECT_EQ(MinorVersion, stream[first_msg_index + 1]);
}

TEST_F(AudioTest, CreateRegisterCommand_getFid_OK) {
  audio aud_command;
  uint8_t MajorVersion = 1;
  uint8_t MinorVersion = 2;
  uint16_t act = 0x1234;
  uint16_t length = 2;
  aud_command.register_command_serialize(act, MajorVersion, MinorVersion,
                                         stream);
  EXPECT_EQ((uint8_t)FID::FID_REGISTER, aud_command.getFid(stream));
}

TEST_F(AudioTest, DeserializeRegisterCommand_deserialize_OK) {
  audio aud_command;
  uint8_t MajorVersion = 1;
  uint8_t MinorVersion = 2;
  uint16_t act = 0x1234;
  aud_command.register_command_serialize(act, MajorVersion, MinorVersion,
                                         stream);

  uint8_t MajorVersion_r = 0;
  uint8_t MinorVersion_r = 0;
  uint16_t act_r = 0;
  aud_command.register_command_deserialize(stream, act_r, MajorVersion_r,
                                           MinorVersion_r);

  EXPECT_EQ(act, act_r);
  EXPECT_EQ(MajorVersion, MajorVersion_r);
  EXPECT_EQ(MinorVersion, MinorVersion_r);
}

TEST_F(AudioTest, CreateRegisterResponse_content_OK) {
  audio aud_response;
  uint8_t MajorVersion = 1;
  uint8_t MinorVersion = 2;
  uint16_t act = 0x1234;
  uint16_t length = 2;
  aud_response.register_response_serialize(act, MajorVersion, MinorVersion,
                                           stream);
  EXPECT_EQ((uint8_t)FID::FID_REGISTER + 1, stream[fid_index]);
  EXPECT_EQ(act, convertToU16(act_index));
  EXPECT_EQ(length, stream[len_index]);
  EXPECT_EQ(MajorVersion, stream[first_msg_index]);
  EXPECT_EQ(MinorVersion, stream[first_msg_index + 1]);
}

TEST_F(AudioTest, CreateRegisterResponse_getFid_OK) {
  audio aud_response;
  uint8_t MajorVersion = 1;
  uint8_t MinorVersion = 2;
  uint16_t act = 0x1234;
  uint16_t length = 2;
  aud_response.register_response_serialize(act, MajorVersion, MinorVersion,
                                           stream);
  EXPECT_EQ((uint8_t)FID::FID_REGISTER + 1, aud_response.getFid(stream));
}

TEST_F(AudioTest, DeserializeRegisterResponse_deserialize_OK) {
  audio aud_response;
  uint8_t MajorVersion = 1;
  uint8_t MinorVersion = 2;
  uint16_t act = 0x1234;
  aud_response.register_response_serialize(act, MajorVersion, MinorVersion,
                                           stream);

  uint8_t MajorVersion_r = 0;
  uint8_t MinorVersion_r = 0;
  uint16_t act_r = 0;
  aud_response.register_response_deserialize(stream, act_r, MajorVersion_r,
                                             MinorVersion_r);

  EXPECT_EQ(act, act_r);
  EXPECT_EQ(MajorVersion, MajorVersion_r);
  EXPECT_EQ(MinorVersion, MinorVersion_r);
}

// setStreamState
TEST_F(AudioTest, CreateSetStreamStateCommand_content_OK) {
  audio aud_command;
  StreamId streamId = StreamId::CIVIC_INFOTAINMENT_STREAM;
  Operation operation = Operation::OP_START;
  uint16_t act = 0x1234;
  uint16_t length = 8;
  aud_command.setStreamState_command_serialize(act, streamId, operation,
                                               stream);
  EXPECT_EQ((uint8_t)FID::FID_SETSTREAMSTATE, stream[fid_index]);
  EXPECT_EQ(act, convertToU16(act_index));
  EXPECT_EQ(length, stream[len_index]);
  EXPECT_EQ((uint8_t)streamId, stream[first_msg_index]);
  EXPECT_EQ((uint8_t)operation, stream[first_msg_index + 4]);
}

TEST_F(AudioTest, CreateSetStreamStateCommand_getFid_OK) {
  audio aud_command;
  StreamId streamId = StreamId::CIVIC_INFOTAINMENT_STREAM;
  Operation operation = Operation::OP_START;
  uint16_t act = 0x1234;
  uint16_t length = 2;
  aud_command.setStreamState_command_serialize(act, streamId, operation,
                                               stream);
  EXPECT_EQ((uint8_t)FID::FID_SETSTREAMSTATE, aud_command.getFid(stream));
}

TEST_F(AudioTest, DeserializeSetStreamStateCommand_deserialize_OK) {
  audio aud_command;
  StreamId streamId = StreamId::CIVIC_INFOTAINMENT_STREAM;
  Operation operation = Operation::OP_START;
  uint16_t act = 0x1234;
  aud_command.setStreamState_command_serialize(act, streamId, operation,
                                               stream);

  StreamId streamId_r = (StreamId)0;
  Operation operation_r = (Operation)0;
  uint16_t act_r = 0;
  aud_command.setStreamState_command_deserialize(stream, act_r, streamId_r,
                                                 operation_r);

  EXPECT_EQ(act, act_r);
  EXPECT_EQ(streamId, streamId_r);
  EXPECT_EQ(operation, operation_r);
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}