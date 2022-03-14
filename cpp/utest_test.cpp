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
  float convertToFloat(const uint8_t& index) {
    return (*(float*)&stream[index]);
  }
  double convertToDouble(const uint8_t& index) {
    return (*(double*)&stream[index]);
  }
  std::vector<uint8_t> convertToU8Vector(const uint8_t& index,
                                         const uint8_t& length) {
    std::vector<uint8_t> output;
    output.clear();
    for (int i = 0; i < length; i++) {
      output.push_back(stream[index + i]);
    }
    return output;
  }
  std::string convertToString(const uint8_t& index, const uint8_t& length) {
    std::string output;
    output.clear();
    for (int i = 0; i < length; i++) {
      output.push_back(stream[index + i]);
    }
    return output;
  }
  ByteBuffer convertToByteBuffer(const uint8_t& index, const uint8_t& length) {
    ByteBuffer output;
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
TEST_F(TestTheTest, CreateRegisterCommand_content_OK) {
  test test_command;
  Version version = { .versionMajor=1, .versionMinor=2, .versionMicro=3};
  uint16_t act = 0x1234;
  uint8_t length = 3;
  test_command.register_command_serialize(act, version, stream);
  EXPECT_EQ((uint8_t)FID::FID_REGISTER, stream[fid_index]);
  EXPECT_EQ(act, convertToU16(act_index));
  EXPECT_EQ(length, stream[len_index]);
  EXPECT_EQ(version.versionMajor, stream[first_msg_index]);
  EXPECT_EQ(version.versionMinor, stream[first_msg_index + 1]);
  EXPECT_EQ(version.versionMicro, stream[first_msg_index + 2]);
}

TEST_F(TestTheTest, CreateRegisterCommand_getFid_OK) {
  test test_command;
  Version version = { .versionMajor=1, .versionMinor=2, .versionMicro=3};
  uint16_t act = 0x1234;
  test_command.register_command_serialize(act, version, stream);
  EXPECT_EQ((uint8_t)FID::FID_REGISTER, test_command.getFid(stream));
}

TEST_F(TestTheTest, DeserializeRegisterCommand_deserialize_OK) {
  test test_command;
  Version version = { .versionMajor=1, .versionMinor=2, .versionMicro=3};
  uint16_t act = 0x1234;
  test_command.register_command_serialize(act, version, stream);

  Version version_r = { .versionMajor=9, .versionMinor=8, .versionMicro=7};
  uint16_t act_r = 0;
  EXPECT_TRUE(
      test_command.register_command_deserialize(stream, act_r, version_r));

  EXPECT_EQ(act, act_r);
  EXPECT_EQ(version.versionMajor, version_r.versionMajor);
  EXPECT_EQ(version.versionMinor, version_r.versionMinor);
  EXPECT_EQ(version.versionMicro, version_r.versionMicro);
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
  EXPECT_TRUE(
      test_command.setEnum_command_deserialize(stream, act_r, testEnum_r));

  EXPECT_EQ(act, act_r);
  EXPECT_EQ(testEnum, testEnum_r);
}
#endif

#if 1  // setIntegerTypes
TEST_F(TestTheTest, CreateSetIntegerTypesCommand_content_OK) {
  test test_command;
  uint16_t act = 0xabba;
  uint8_t u8Var = 0x12;
  uint16_t u16Var = 0x1234;
  uint32_t u32Var = 0x12345678;
  uint64_t u64Var = 0x1234567887654321;
  uint16_t length =
      sizeof(u8Var) + sizeof(u16Var) + sizeof(u32Var) + sizeof(u64Var);
  test_command.setIntegerTypes_command_serialize(act, u8Var, u16Var, u32Var,
                                                 u64Var, stream);
  EXPECT_EQ(act, convertToU16(act_index));
  EXPECT_EQ(length, convertToU16(len_index));
  EXPECT_EQ(u8Var, stream[first_msg_index]);
  EXPECT_EQ(u16Var, convertToU16(first_msg_index + 1));
  EXPECT_EQ(u32Var, convertToU32(first_msg_index + 3));
  EXPECT_EQ(u64Var, convertToU64(first_msg_index + 7));
}

TEST_F(TestTheTest, CreateSetIntegerTypesCommand_getFid_OK) {
  test test_command;
  uint16_t act = 0xabba;
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
  EXPECT_TRUE(test_command.setIntegerTypes_command_deserialize(
      stream, act_r, u8Var_r, u16Var_r, u32Var_r, u64Var_r));

  EXPECT_EQ(act, act_r);
  EXPECT_EQ(u8Var, u8Var_r);
  EXPECT_EQ(u16Var, u16Var_r);
  EXPECT_EQ(u32Var, u32Var_r);
  EXPECT_EQ(u64Var, u64Var_r);
}

TEST_F(TestTheTest, CreateSetIntegerTypesResponse_content_OK) {
  test test_command;
  uint16_t act = 0xabba;
  uint8_t u8Var = 0x12;
  uint16_t u16Var = 0x1234;
  uint32_t u32Var = 0x12345678;
  uint64_t u64Var = 0x1234567887654321;
  uint16_t length =
      sizeof(u8Var) + sizeof(u16Var) + sizeof(u32Var) + sizeof(u64Var);
  test_command.setIntegerTypes_response_serialize(act, u64Var, u32Var, u16Var,
                                                  u8Var, stream);
  EXPECT_EQ(act, convertToU16(act_index));
  EXPECT_EQ(length, convertToU16(len_index));
  EXPECT_EQ(u64Var, convertToU64(first_msg_index));
  EXPECT_EQ(u32Var, convertToU32(first_msg_index + 4));
  EXPECT_EQ(u16Var, convertToU16(first_msg_index + 6));
  EXPECT_EQ(u8Var, stream[first_msg_index + 7]);
}

TEST_F(TestTheTest, CreateSetIntegerTypesResponse_getFid_OK) {
  test test_command;
  uint16_t act = 0xabba;
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
  uint8_t u8Var = 0x12;
  uint16_t u16Var = 0x1234;
  uint32_t u32Var = 0x12345678;
  uint64_t u64Var = 0x1234567887654321;
  test_command.setIntegerTypes_response_serialize(act, u64Var, u32Var, u16Var,
                                                  u8Var, stream);

  uint16_t act_r = 0;
  uint8_t u8Var_r = 0;
  uint16_t u16Var_r = 0;
  uint32_t u32Var_r = 0;
  uint64_t u64Var_r = 05;
  EXPECT_TRUE(test_command.setIntegerTypes_response_deserialize(
      stream, act_r, u64Var_r, u32Var_r, u16Var_r, u8Var_r));

  EXPECT_EQ(act, act_r);
  EXPECT_EQ(u8Var, u8Var_r);
  EXPECT_EQ(u16Var, u16Var_r);
  EXPECT_EQ(u32Var, u32Var_r);
  EXPECT_EQ(u64Var, u64Var_r);
}
#endif

#if 1  // setStructTypes
TEST_F(TestTheTest, CreateSetStructTypesCommand_content_OK) {
  test test_command;
  uint16_t act = 0xabba;
  TestStruct testStruct;
  testStruct.u8TestVar = 0x12;
  testStruct.u16TestVar = 0x3456;
  testStruct.bTestBool = true;
  testStruct.testEnum = TestEnum1::TESTENUM1_100;
  uint16_t length = sizeof(testStruct);
  test_command.setStruct_command_serialize(act, testStruct, stream);
  EXPECT_EQ(act, convertToU16(act_index));
  EXPECT_EQ(length, convertToU16(len_index));
}

TEST_F(TestTheTest, CreateSetStructTypesCommand_getFid_OK) {
  test test_command;
  uint16_t act = 0xabba;
  TestStruct testStruct;
  testStruct.u8TestVar = 0x12;
  testStruct.u16TestVar = 0x3456;
  testStruct.bTestBool = true;
  testStruct.testEnum = TestEnum1::TESTENUM1_100;
  test_command.setStruct_command_serialize(act, testStruct, stream);
  EXPECT_EQ((uint8_t)FID::FID_SETSTRUCT, test_command.getFid(stream));
}

TEST_F(TestTheTest, DeserializeSetStructTypesCommand_deserialize_OK) {
  test test_command;
  uint16_t act = 0xabba;
  TestStruct testStruct;
  testStruct.u8TestVar = 0x12;
  testStruct.u16TestVar = 0x3456;
  testStruct.bTestBool = true;
  testStruct.testEnum = TestEnum1::TESTENUM1_100;
  test_command.setStruct_command_serialize(act, testStruct, stream);

  uint16_t act_r = 0;
  TestStruct testStruct_r;
  EXPECT_TRUE(
      test_command.setStruct_command_deserialize(stream, act_r, testStruct_r));

  EXPECT_EQ(act, act_r);
  EXPECT_EQ(testStruct.u8TestVar, testStruct_r.u8TestVar);
  EXPECT_EQ(testStruct.u16TestVar, testStruct_r.u16TestVar);
  EXPECT_EQ(testStruct.bTestBool, testStruct_r.bTestBool);
  EXPECT_EQ(testStruct.testEnum, testStruct_r.testEnum);
}

#endif

#if 1  // setFloatingPointTypes
TEST_F(TestTheTest, CreateSetFloatingPointTypesCommand_content_OK) {
  test test_command;
  uint16_t act = 0xabba;
  float floatVar = 1.2345;
  double doubleVar = 98765.43210;
  uint16_t length = sizeof(floatVar) + sizeof(doubleVar);
  test_command.setFloatingPointTypes_command_serialize(act, floatVar, doubleVar,
                                                       stream);
  EXPECT_EQ(act, convertToU16(act_index));
  EXPECT_EQ(length, convertToU16(len_index));
  EXPECT_EQ(floatVar, convertToFloat(first_msg_index));
  EXPECT_EQ(doubleVar, convertToDouble(first_msg_index + sizeof(floatVar)));
}

TEST_F(TestTheTest, CreateSetFloatingPointTypesCommand_getFid_OK) {
  test test_command;
  uint16_t act = 0xabba;
  float floatVar = 1.2345;
  double doubleVar = 98765.43210;
  test_command.setFloatingPointTypes_command_serialize(act, floatVar, doubleVar,
                                                       stream);
  EXPECT_EQ((uint8_t)FID::FID_SETFLOATINGPOINTTYPES,
            test_command.getFid(stream));
}

TEST_F(TestTheTest, DeserializeSetFloatingPointTypesCommand_deserialize_OK) {
  test test_command;
  uint16_t act = 0xabba;
  float floatVar = 1.2345;
  double doubleVar = 98765.43210;
  test_command.setFloatingPointTypes_command_serialize(act, floatVar, doubleVar,
                                                       stream);

  uint16_t act_r = 0;
  float floatVar_r = 1.2345;
  double doubleVar_r = 9876.54321;
  EXPECT_TRUE(test_command.setFloatingPointTypes_command_deserialize(
      stream, act_r, floatVar_r, doubleVar_r));

  EXPECT_EQ(act, act_r);
  EXPECT_EQ(floatVar, floatVar_r);
  EXPECT_EQ(doubleVar, doubleVar_r);
}

TEST_F(TestTheTest, CreateSetFloatingPointTypesResponse_content_OK) {
  test test_command;
  uint16_t act = 0xabba;
  float floatVar = 1.2345;
  double doubleVar = 98765.43210;
  uint16_t length = sizeof(floatVar) + sizeof(doubleVar);
  test_command.setFloatingPointTypes_response_serialize(act, doubleVar,
                                                        floatVar, stream);
  EXPECT_EQ(act, convertToU16(act_index));
  EXPECT_EQ(length, convertToU16(len_index));
  EXPECT_EQ(doubleVar, convertToDouble(first_msg_index));
  EXPECT_EQ(floatVar, convertToFloat(first_msg_index + sizeof(doubleVar)));
}

TEST_F(TestTheTest, CreateSetFloatingPointTypesResponse_getFid_OK) {
  test test_command;
  uint16_t act = 0xabba;
  float floatVar = 1.2345;
  double doubleVar = 98765.43210;
  test_command.setFloatingPointTypes_response_serialize(act, doubleVar,
                                                        floatVar, stream);
  EXPECT_EQ((uint8_t)FID::FID_SETFLOATINGPOINTTYPES + 1,
            test_command.getFid(stream));
}

TEST_F(TestTheTest, DeserializeSetFloatingPointTypesResponse_deserialize_OK) {
  test test_command;
  uint16_t act = 0xabba;
  uint8_t u8Var = 0x12;
  float floatVar = 1.2345;
  double doubleVar = 98765.43210;
  test_command.setFloatingPointTypes_response_serialize(act, doubleVar,
                                                        floatVar, stream);

  uint16_t act_r = 0;
  float floatVar_r = 5.4321;
  double doubleVar_r = 12345.67890;
  EXPECT_TRUE(test_command.setFloatingPointTypes_response_deserialize(
      stream, act_r, doubleVar_r, floatVar_r));

  EXPECT_EQ(act, act_r);
  EXPECT_EQ(doubleVar, doubleVar_r);
  EXPECT_EQ(floatVar, floatVar_r);
}
#endif

#if 1  // setIntegerArrayTypes
TEST_F(TestTheTest, CreateSetIntegerArrayTypesCommand_content_OK) {
  test test_command;
  uint16_t act = 0xDCBA;
  std::vector<uint8_t> testVector{'H', 'a', 'l', 'l', 'o'};
  uint8_t u8Var = 0xFF;
  uint8_t length = sizeof(u8Var) + 2 + testVector.size();
  test_command.setIntegerArrayTypes_command_serialize(act, u8Var, testVector,
                                                      stream);
  EXPECT_EQ(act, convertToU16(act_index));
  EXPECT_EQ(length, stream[len_index]);
  EXPECT_EQ(u8Var, stream[first_msg_index]);
  EXPECT_EQ(testVector.size(), stream[first_msg_index + 1]);
  EXPECT_THAT(convertToU8Vector(first_msg_index + 3, testVector.size()),
              ::testing::ContainerEq(testVector));
}

TEST_F(TestTheTest, CreateSetIntegerArrayTypesCommand_getFid_OK) {
  test test_command;
  uint16_t act = 0xDCBA;
  std::vector<uint8_t> testVector{'H', 'a', 'l', 'l', 'o'};
  uint8_t u8Var = 0xCC;
  test_command.setIntegerArrayTypes_command_serialize(act, u8Var, testVector,
                                                      stream);
  EXPECT_EQ((uint8_t)FID::FID_SETINTEGERARRAYTYPES,
            test_command.getFid(stream));
}

TEST_F(TestTheTest, DeserializeSetIntegerArrayTypesCommand_deserialize_OK) {
  test test_command;
  uint16_t act = 0xDCBA;
  std::vector<uint8_t> testVector{'H', 'a', 'l', 'l', 'o'};
  uint8_t u8Var = 0xFF;
  test_command.setIntegerArrayTypes_command_serialize(act, u8Var, testVector,
                                                      stream);

  uint16_t act_r = 0;
  std::vector<uint8_t> testVector_r;
  uint8_t u8Var_r = 0;
  EXPECT_TRUE(test_command.setIntegerArrayTypes_command_deserialize(
      stream, act_r, u8Var_r, testVector_r));

  EXPECT_EQ(act, act_r);
  EXPECT_EQ(u8Var, u8Var_r);
  EXPECT_TRUE(testVector == testVector_r);
}

TEST_F(TestTheTest, CreateSetIntegerArrayTypesResponse_content_OK) {
  test test_command;
  uint16_t act = 0xDCBA;
  std::vector<uint8_t> testVector{'H', 'a', 'l', 'l', 'o'};
  uint8_t u8Var = 0xAA;
  uint8_t length = sizeof(u8Var) + 2 + testVector.size();
  test_command.setIntegerArrayTypes_response_serialize(act, testVector, u8Var,
                                                       stream);
  EXPECT_EQ(act, convertToU16(act_index));
  EXPECT_EQ(length, stream[len_index]);
  EXPECT_EQ(testVector.size(), convertToU16(first_msg_index));
  EXPECT_THAT(convertToU8Vector(first_msg_index + 2, testVector.size()),
              ::testing::ContainerEq(testVector));
  EXPECT_EQ(u8Var, stream[first_msg_index + 2 + testVector.size()]);
}

TEST_F(TestTheTest, CreateSetIntegerArrayTypesResponse_getFid_OK) {
  test test_command;
  uint16_t act = 0xDCBA;
  std::vector<uint8_t> testVector{'H', 'a', 'l', 'l', 'o'};
  uint8_t u8Var = 0xBB;
  test_command.setIntegerArrayTypes_response_serialize(act, testVector, u8Var,
                                                       stream);
  EXPECT_EQ((uint8_t)FID::FID_SETINTEGERARRAYTYPES + 1,
            test_command.getFid(stream));
}

TEST_F(TestTheTest, DeserializeSetIntegerArrayTypesResponse_deserialize_OK) {
  test test_command;
  uint16_t act = 0xDCBA;
  std::vector<uint8_t> testVector{'H', 'a', 'l', 'l', 'o'};
  uint8_t u8Var = testVector.size();
  test_command.setIntegerArrayTypes_response_serialize(act, testVector, u8Var,
                                                       stream);

  uint16_t act_r = 0;
  std::vector<uint8_t> testVector_r;
  uint8_t u8Var_r = 0;
  EXPECT_TRUE(test_command.setIntegerArrayTypes_response_deserialize(
      stream, act_r, testVector_r, u8Var_r));

  EXPECT_EQ(act, act_r);
  EXPECT_EQ(u8Var, u8Var_r);
  EXPECT_TRUE(testVector == testVector_r);
}
#endif

#if 1  // setDynamicTypes
TEST_F(TestTheTest, CreateSetDynamicTypesCommand_content_OK) {
  test test_command;
  uint16_t act = 0xDCBA;
  std::string TestString("Hallo you");
  uint16_t u8StringLength = TestString.size();
  uint64_t u64TestVar = 0x1010101020202020;
  uint8_t length = 2 + u8StringLength + sizeof(u64TestVar);
  test_command.setDynamicTypes_command_serialize(act, TestString, u64TestVar,
                                                 stream);
  EXPECT_EQ(act, convertToU16(act_index));
  EXPECT_EQ(length, stream[len_index]);
  EXPECT_EQ(u8StringLength, convertToU16(first_msg_index));
  EXPECT_THAT(convertToString(first_msg_index + 2, u8StringLength),
              ::testing::ContainerEq(TestString));
}

TEST_F(TestTheTest, CreateSetDynamicTypesCommand_getFid_OK) {
  test test_command;
  uint16_t act = 0xDCBA;
  std::string TestString("Hallo you");
  uint8_t u8StringLength = TestString.size();
  uint64_t u64TestVar = 0x1010101020202020;
  uint8_t length = 1 + u8StringLength;
  test_command.setDynamicTypes_command_serialize(act, TestString, u64TestVar,
                                                 stream);
  EXPECT_EQ((uint8_t)FID::FID_SETDYNAMICTYPES, test_command.getFid(stream));
}

TEST_F(TestTheTest, DeserializeSetDynamicTypesCommand_deserialize_OK) {
  test test_command;
  uint16_t act = 0xDCBA;
  std::string TestString("Hallo you");
  uint16_t u8StringLength = TestString.size();
  uint64_t u64TestVar = 0x1010101020202020;
  uint8_t length = 1 + u8StringLength;
  test_command.setDynamicTypes_command_serialize(act, TestString, u64TestVar,
                                                 stream);

  uint16_t act_r = 0;
  std::string TestString_r("");
  uint64_t u64TestVar_r = 0;
  EXPECT_TRUE(test_command.setDynamicTypes_command_deserialize(
      stream, act_r, TestString_r, u64TestVar_r));

  EXPECT_EQ(act, act_r);
  EXPECT_EQ(u64TestVar, u64TestVar_r);
  EXPECT_TRUE(TestString == TestString_r);
}

TEST_F(TestTheTest, CreateSetDynamicTypesResponse_content_OK) {
  test test_command;
  uint16_t act = 0xDCBA;
  uint32_t u32TestVar = 0x10101010;
  ByteBuffer testBuffer{'H', 'a', 'l', 'l', 'i', 'H', 'a', 'l', 'l', 'o'};
  uint8_t bufferSize = testBuffer.size();
  uint8_t length = 2 + bufferSize + sizeof(u32TestVar);
  test_command.setDynamicTypes_response_serialize(act, testBuffer, u32TestVar,
                                                  stream);

  EXPECT_EQ(act, convertToU16(act_index));
  EXPECT_EQ(length, stream[len_index]);
  EXPECT_EQ(bufferSize, convertToU16(first_msg_index));
  EXPECT_THAT(convertToByteBuffer(first_msg_index + 2, bufferSize),
              ::testing::ContainerEq(testBuffer));
  EXPECT_EQ(u32TestVar, convertToU32(first_msg_index + 2 + bufferSize));
}

TEST_F(TestTheTest, CreateSetDynamicTypesResponse_getFid_OK) {
  test test_command;
  uint16_t act = 0xDCBA;
  ByteBuffer testBuffer{'H', 'a', 'l', 'l', 'i', 'H', 'a', 'l', 'l', 'o'};
  uint32_t u32TestVar = 0x09900990;
  uint8_t length = 2 + testBuffer.size();
  test_command.setDynamicTypes_response_serialize(act, testBuffer, u32TestVar,
                                                  stream);
  EXPECT_EQ((uint8_t)FID::FID_SETDYNAMICTYPES + 1, test_command.getFid(stream));
}

TEST_F(TestTheTest, DeserializeSetDynamicTypesResponse_deserialize_OK) {
  test test_command;
  uint16_t act = 0xDCBA;
  ByteBuffer testBuffer{'H', 'a', 'l', 'l', 'i', 'H', 'a', 'l', 'l', 'o'};
  uint32_t u32TestVar = 0x09900990;
  uint8_t u8BufferLength = testBuffer.size();
  uint8_t length = 1 + u8BufferLength;
  test_command.setDynamicTypes_response_serialize(act, testBuffer, u32TestVar,
                                                  stream);

  uint16_t act_r = 0;
  ByteBuffer testBuffer_r;
  uint32_t u32Var_r = 0;
  EXPECT_TRUE(test_command.setDynamicTypes_response_deserialize(
      stream, act_r, testBuffer_r, u32Var_r));

  EXPECT_EQ(act, act_r);
  EXPECT_EQ(u32TestVar, u32Var_r);
  EXPECT_TRUE(testBuffer == testBuffer_r);
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
  EXPECT_TRUE(
      test_command.setBoolTypes_command_deserialize(stream, act_r, boolVar_r));

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
  EXPECT_TRUE(
      test_command.setBoolTypes_response_deserialize(stream, act_r, boolVar_r));

  EXPECT_EQ(act, act_r);
  EXPECT_EQ(boolVar, boolVar_r);
}
#endif

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}