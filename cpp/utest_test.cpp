#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "test.h"

class TestTheTest : public ::testing::Test {
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
  std::vector<uint8_t> convertToUu8Vector(const uint8_t& index,
                                          const uint8_t& length) {
    std::vector<uint8_t> output;
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
  const uint8_t first_msg_index = 4;
};

#if 1  // register
TEST_F(TestTheTest, CreateRegisterCommand_content_OK) {
  test test_command;
  uint8_t MajorVersion = 1;
  uint8_t MinorVersion = 2;
  uint16_t act = 0x1234;
  uint8_t length = 2;
  test_command.register_command_serialize(act, MajorVersion, MinorVersion,
                                          stream);
  EXPECT_EQ((uint8_t)FID::FID_REGISTER, stream[fid_index]);
  EXPECT_EQ(act, convertToU16(act_index));
  EXPECT_EQ(length, stream[len_index]);
  EXPECT_EQ(MajorVersion, stream[first_msg_index]);
  EXPECT_EQ(MinorVersion, stream[first_msg_index + 1]);
}

TEST_F(TestTheTest, CreateRegisterCommand_getFid_OK) {
  test test_command;
  uint8_t MajorVersion = 1;
  uint8_t MinorVersion = 2;
  uint16_t act = 0x1234;
  uint8_t length = 2;
  test_command.register_command_serialize(act, MajorVersion, MinorVersion,
                                          stream);
  EXPECT_EQ((uint8_t)FID::FID_REGISTER, test_command.getFid(stream));
}

TEST_F(TestTheTest, DeserializeRegisterCommand_deserialize_OK) {
  test test_command;
  uint8_t MajorVersion = 1;
  uint8_t MinorVersion = 2;
  uint16_t act = 0x1234;
  test_command.register_command_serialize(act, MajorVersion, MinorVersion,
                                          stream);

  uint8_t MajorVersion_r = 0;
  uint8_t MinorVersion_r = 0;
  uint16_t act_r = 0;
  test_command.register_command_deserialize(stream, act_r, MajorVersion_r,
                                            MinorVersion_r);

  EXPECT_EQ(act, act_r);
  EXPECT_EQ(MajorVersion, MajorVersion_r);
  EXPECT_EQ(MinorVersion, MinorVersion_r);
}
#endif

#if 1  // setEnum
TEST_F(TestTheTest, CreateSetEnumCommand_content_OK) {
  test test_command;
  TestEnum1 testEnum = TestEnum1::TESTENUM1_10;
  uint16_t act = 0x1234;
  uint8_t length = 4;
  test_command.setEnum_command_serialize(act, testEnum, stream);
  EXPECT_EQ((uint8_t)FID::FID_SETENUM, stream[fid_index]);
  EXPECT_EQ(act, convertToU16(act_index));
  EXPECT_EQ(length, stream[len_index]);
  EXPECT_EQ((uint8_t)testEnum, stream[first_msg_index]);
}

TEST_F(TestTheTest, CreateSetEnumCommand_getFid_OK) {
  test test_command;
  TestEnum1 testEnum = TestEnum1::TESTENUM1_10;
  uint16_t act = 0x1234;
  uint8_t length = 2;
  test_command.setEnum_command_serialize(act, testEnum, stream);
  EXPECT_EQ((uint8_t)FID::FID_SETENUM, test_command.getFid(stream));
}

TEST_F(TestTheTest, DeserializeSetEnumCommand_deserialize_OK) {
  test test_command;
  TestEnum1 testEnum = TestEnum1::TESTENUM1_10;
  uint16_t act = 0x1234;
  test_command.setEnum_command_serialize(act, testEnum, stream);

  TestEnum1 testEnum_r = (TestEnum1)0;
  uint16_t act_r = 0;
  test_command.setEnum_command_deserialize(stream, act_r, testEnum_r);

  EXPECT_EQ(act, act_r);
  EXPECT_EQ(testEnum, testEnum_r);
}
#endif

#if 1  // setIntegerTypes
TEST_F(TestTheTest, CreateSetIntegerTypesCommand_content_OK) {
  test test_command;
  uint16_t act = 0xabba;
  uint8_t length = 1 + 2 + 4 + 8;
  uint8_t u8Var = 0x12;
  uint16_t u16Var = 0x1234;
  uint32_t u32Var = 0x12345678;
  uint64_t u64Var = 0x1234567887654321;
  test_command.setIntegerTypes_command_serialize(act, u8Var, u16Var, u32Var,
                                                 u64Var, stream);
  EXPECT_EQ(act, convertToU16(act_index));
  EXPECT_EQ(length, stream[len_index]);
  EXPECT_EQ(u8Var, stream[first_msg_index]);
  EXPECT_EQ(u16Var, convertToU16(first_msg_index + 1));
  EXPECT_EQ(u32Var, convertToU32(first_msg_index + 3));
  EXPECT_EQ(u64Var, convertToU64(first_msg_index + 7));
}

TEST_F(TestTheTest, CreateSetIntegerTypesCommand_getFid_OK) {
  test test_command;
  uint16_t act = 0xabba;
  uint8_t length = 1 + 4 + 8 + 16;
  uint8_t u8Var = 0x12;
  uint16_t u16Var = 0x1234;
  uint32_t u32Var = 0x12345678;
  uint64_t u64Var = 0x1234567887654321;
  test_command.setIntegerTypes_command_serialize(act, u8Var, u16Var, u32Var,
                                                 u64Var, stream);
  EXPECT_EQ((uint8_t)FID::FID_SETINTEGERTYPES, test_command.getFid(stream));
}

TEST_F(TestTheTest, DeserializeSetIntegerTypesCommand_deserialize_OK) {
  test test_command;
  uint16_t act = 0xabba;
  uint8_t length = 1 + 4 + 8 + 16;
  uint8_t u8Var = 0x12;
  uint16_t u16Var = 0x1234;
  uint32_t u32Var = 0x12345678;
  uint64_t u64Var = 0x1234567887654321;
  test_command.setIntegerTypes_command_serialize(act, u8Var, u16Var, u32Var,
                                                 u64Var, stream);

  uint16_t act_r = 0;
  uint8_t u8Var_r = 0x12;
  uint16_t u16Var_r = 0x1234;
  uint32_t u32Var_r = 0x12345678;
  uint64_t u64Var_r = 0x1234567887654321;
  test_command.setIntegerTypes_command_deserialize(
      stream, act_r, u8Var_r, u16Var_r, u32Var_r, u64Var_r);

  EXPECT_EQ(act, act_r);
  EXPECT_EQ(u8Var, u8Var_r);
  EXPECT_EQ(u16Var, u16Var_r);
  EXPECT_EQ(u32Var, u32Var_r);
  EXPECT_EQ(u64Var, u64Var_r);
}

TEST_F(TestTheTest, CreateSetIntegerTypesResponse_content_OK) {
  test test_command;
  uint16_t act = 0xabba;
  uint8_t length = 1 + 2 + 4 + 8;
  uint8_t u8Var = 0x12;
  uint16_t u16Var = 0x1234;
  uint32_t u32Var = 0x12345678;
  uint64_t u64Var = 0x1234567887654321;
  test_command.setIntegerTypes_response_serialize(act, u64Var, u32Var, u16Var,
                                                  u8Var, stream);
  EXPECT_EQ(act, convertToU16(act_index));
  EXPECT_EQ(length, stream[len_index]);
  EXPECT_EQ(u64Var, convertToU64(first_msg_index));
  EXPECT_EQ(u32Var, convertToU32(first_msg_index + 4));
  EXPECT_EQ(u16Var, convertToU16(first_msg_index + 6));
  EXPECT_EQ(u8Var, stream[first_msg_index + 7]);
}

TEST_F(TestTheTest, CreateSetIntegerTypesResponse_getFid_OK) {
  test test_command;
  uint16_t act = 0xabba;
  uint8_t length = 1 + 4 + 8 + 16;
  uint8_t u8Var = 0x12;
  uint16_t u16Var = 0x1234;
  uint32_t u32Var = 0x12345678;
  uint64_t u64Var = 0x1234567887654321;
  test_command.setIntegerTypes_response_serialize(act, u64Var, u32Var, u16Var,
                                                  u8Var, stream);
  EXPECT_EQ((uint8_t)FID::FID_SETINTEGERTYPES + 1, test_command.getFid(stream));
}

TEST_F(TestTheTest, DeserializeSetIntegerTypesResponse_deserialize_OK) {
  test test_command;
  uint16_t act = 0xabba;
  uint8_t length = 1 + 4 + 8 + 16;
  uint8_t u8Var = 0x12;
  uint16_t u16Var = 0x1234;
  uint32_t u32Var = 0x12345678;
  uint64_t u64Var = 0x1234567887654321;
  test_command.setIntegerTypes_response_serialize(act, u64Var, u32Var, u16Var,
                                                  u8Var, stream);

  uint16_t act_r = 0;
  uint8_t u8Var_r = 0x12;
  uint16_t u16Var_r = 0x1234;
  uint32_t u32Var_r = 0x12345678;
  uint64_t u64Var_r = 0x1234567887654321;
  test_command.setIntegerTypes_response_deserialize(
      stream, act_r, u64Var_r, u32Var_r, u16Var_r, u8Var_r);

  EXPECT_EQ(act, act_r);
  EXPECT_EQ(u8Var, u8Var_r);
  EXPECT_EQ(u16Var, u16Var_r);
  EXPECT_EQ(u32Var, u32Var_r);
  EXPECT_EQ(u64Var, u64Var_r);
}
#endif

#if 1  // setIntegerArrayTypes
TEST_F(TestTheTest, CreateSetIntegerArrayTypesCommand_content_OK) {
  test test_command;
  uint16_t act = 0xDCBA;
  std::vector<uint8_t> testVector{'H', 'a', 'l', 'l', 'o'};
  uint8_t u8VarLength = testVector.size();
  uint8_t length = 1 + u8VarLength;
  test_command.setIntegerArrayTypes_command_serialize(act, u8VarLength,
                                                      testVector, stream);
  EXPECT_EQ(act, convertToU16(act_index));
  EXPECT_EQ(length, stream[len_index]);
  EXPECT_EQ(u8VarLength, stream[first_msg_index]);
  EXPECT_THAT(convertToUu8Vector(first_msg_index + 1, u8VarLength),
              ::testing::ContainerEq(testVector));
}

TEST_F(TestTheTest, CreateSetIntegerArrayTypesCommand_getFid_OK) {
  test test_command;
  uint16_t act = 0xDCBA;
  std::vector<uint8_t> testVector{'H', 'a', 'l', 'l', 'o'};
  uint8_t u8VarLength = testVector.size();
  uint8_t length = 1 + u8VarLength;
  test_command.setIntegerArrayTypes_command_serialize(act, u8VarLength,
                                                      testVector, stream);
  EXPECT_EQ((uint8_t)FID::FID_SETINTEGERARRAYTYPES, test_command.getFid(stream));
}

TEST_F(TestTheTest, DeserializeSetIntegerArrayTypesCommand_deserialize_OK) {
  test test_command;
  uint16_t act = 0xDCBA;
  std::vector<uint8_t> testVector{'H', 'a', 'l', 'l', 'o'};
  uint8_t u8VarLength = testVector.size();
  uint8_t length = 1 + u8VarLength;
  test_command.setIntegerArrayTypes_command_serialize(act, u8VarLength,
                                                      testVector, stream);

  uint16_t act_r = 0;
  std::vector<uint8_t> testVector_r;
  uint8_t u8VarLength_r = 0;
  test_command.setIntegerArrayTypes_command_deserialize(
      stream, act_r, u8VarLength_r, testVector_r);

  EXPECT_EQ(act, act_r);
  EXPECT_EQ(u8VarLength, u8VarLength_r);
  EXPECT_TRUE(testVector == testVector_r);
}

TEST_F(TestTheTest, CreateSetIntegerArrayTypesResponse_content_OK) {
  test test_command;
  uint16_t act = 0xDCBA;
  std::vector<uint64_t> testVector{'H', 'a', 'l', 'l', 'o'};
  uint8_t u8VarLength = testVector.size();
  uint8_t length = 1 + u8VarLength;
  test_command.setIntegerArrayTypes_response_serialize(act, u8VarLength,
                                                       testVector, stream);
}

TEST_F(TestTheTest, CreateSetIntegerArrayTypesResponse_getFid_OK) {
  test test_command;
  uint16_t act = 0xDCBA;
  std::vector<uint64_t> testVector{'H', 'a', 'l', 'l', 'o'};
  uint8_t u8VarLength = testVector.size();
  uint8_t length = 1 + u8VarLength;
  test_command.setIntegerArrayTypes_response_serialize(act, u8VarLength,
                                                       testVector, stream);
  EXPECT_EQ((uint8_t)FID::FID_SETINTEGERARRAYTYPES + 1, test_command.getFid(stream));
}

TEST_F(TestTheTest, DeserializeSetIntegerArrayTypesResponse_deserialize_OK) {
  test test_command;
  uint16_t act = 0xDCBA;
  std::vector<uint64_t> testVector{'H', 'a', 'l', 'l', 'o'};
  uint8_t u8VarLength = testVector.size();
  uint8_t length = 1 + u8VarLength;
  test_command.setIntegerArrayTypes_response_serialize(act, u8VarLength,
                                                       testVector, stream);
}
#endif

#if 1  // setBoolTypes
TEST_F(TestTheTest, CreateSetBoolTypesCommand_content_OK) {
  test test_command;
  uint16_t act = 0xb001;
  uint8_t length = 1;
  bool boolVar = true;
  test_command.setBoolTypes_command_serialize(act, boolVar, stream);
  EXPECT_EQ(act, convertToU16(act_index));
  EXPECT_EQ(length, stream[len_index]);
  EXPECT_EQ(boolVar, stream[first_msg_index]);
}

TEST_F(TestTheTest, CreateSetBoolTypesCommand_getFid_OK) {
  test test_command;
  uint16_t act = 0xb001;
  uint8_t length = 1;
  bool boolVar = true;
  test_command.setBoolTypes_command_serialize(act, boolVar, stream);
  EXPECT_EQ((uint8_t)FID::FID_SETBOOLTYPES, test_command.getFid(stream));
}

TEST_F(TestTheTest, DeserializeSetBoolTypesCommand_deserialize_OK) {
  test test_command;
  uint16_t act = 0xb001;
  uint8_t length = 1;
  bool boolVar = true;
  test_command.setBoolTypes_command_serialize(act, boolVar, stream);

  uint16_t act_r = 0;
  bool boolVar_r = false;
  test_command.setBoolTypes_command_deserialize(stream, act_r, boolVar_r);

  EXPECT_EQ(act, act_r);
  EXPECT_EQ(boolVar, boolVar_r);
}

TEST_F(TestTheTest, CreateSetBoolTypesResponse_content_OK) {
  test test_command;
  uint16_t act = 0xb001;
  uint8_t length = 1;
  bool boolVar = true;
  test_command.setBoolTypes_response_serialize(act, boolVar, stream);
  EXPECT_EQ(act, convertToU16(act_index));
  EXPECT_EQ(length, stream[len_index]);
  EXPECT_EQ(boolVar, stream[first_msg_index]);
}

TEST_F(TestTheTest, CreateSetBoolTypesResponse_getFid_OK) {
  test test_command;
  uint16_t act = 0xb001;
  uint8_t length = 1;
  bool boolVar = true;
  test_command.setBoolTypes_response_serialize(act, boolVar, stream);
  EXPECT_EQ((uint8_t)FID::FID_SETBOOLTYPES + 1, test_command.getFid(stream));
}

TEST_F(TestTheTest, DeserializeSetBoolTypesResponse_deserialize_OK) {
  test test_command;
  uint16_t act = 0xb001;
  uint8_t length = 1;
  bool boolVar = true;
  test_command.setBoolTypes_response_serialize(act, boolVar, stream);

  uint16_t act_r = 0;
  bool boolVar_r = false;
  test_command.setBoolTypes_response_deserialize(stream, act_r, boolVar_r);

  EXPECT_EQ(act, act_r);
  EXPECT_EQ(boolVar, boolVar_r);
}
#endif

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}