#include <vector>

#include "audio.h"
#include "gtest/gtest.h"

class AudioTest : public ::testing::Test {
 protected:
  void SetUp() override {}
  void TearDown() override {}
  uint16_t convertTo16(const uint8_t& index) {
    return (stream[index]) + (((uint16_t)stream[index + 1]) << 8);
  }
  uint32_t convertTo32(const uint8_t& index) {
    return (convertTo16(index)) + (((uint32_t)convertTo16(index + 2)) << 16);
  }
  uint64_t convertTo64(const uint8_t& index) {
    return (convertTo32(index)) + (((uint64_t)(index + 4)) << 32);
  }
  std::vector<uint8_t> stream;
  const uint8_t fid_index = 0;
  const uint8_t act_index = 1;
  const uint8_t len_index = 3;
  const uint8_t first_msg_index = 4;
};

TEST_F(AudioTest, CreateRegisterCommand_content_OK) {
  audio aud_command_send;
  uint8_t MajorVersion = 1;
  uint8_t MinorVersion = 2;
  uint16_t act = 0x1234;
  uint8_t length = 2;
  aud_command_send.register_command_serialize(act, MajorVersion, MinorVersion,
                                              stream);
  EXPECT_EQ((uint8_t)FID::FID_REGISTER, stream[fid_index]);
  EXPECT_EQ(act, convertTo16(act_index));
  EXPECT_EQ(length, stream[len_index]);
  EXPECT_EQ(MajorVersion, stream[first_msg_index]);
  EXPECT_EQ(MinorVersion, stream[first_msg_index + 1]);
}

TEST_F(AudioTest, CreateRegisterCommand_getFid_OK) {
  audio aud_command;
  uint8_t MajorVersion = 1;
  uint8_t MinorVersion = 2;
  uint16_t act = 0x1234;
  uint8_t length = 2;
  aud_command.register_command_serialize(act, MajorVersion, MinorVersion,
                                         stream);
  EXPECT_EQ((uint8_t)FID::FID_REGISTER, aud_command.getFid(stream));
}

TEST_F(AudioTest, DeserializeRegisterCommand_deserialize_OK) {
  audio aud_command_send;
  uint8_t MajorVersion = 1;
  uint8_t MinorVersion = 2;
  uint16_t act = 0x1234;
  aud_command_send.register_command_serialize(act, MajorVersion, MinorVersion,
                                              stream);

  uint8_t MajorVersion_r = 0;
  uint8_t MinorVersion_r = 0;
  uint16_t act_r = 0;
  aud_command_send.register_command_deserialize(stream, act_r, MajorVersion_r,
                                                MinorVersion_r);

  EXPECT_EQ(act, act_r);
  EXPECT_EQ(MajorVersion, MajorVersion_r);
  EXPECT_EQ(MinorVersion, MinorVersion_r);
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}