#include <gtest/gtest.h>
#include <json/json_object.h>

using namespace std;
using namespace json;

TEST(JsonObjectTests, EmptyJsonTest)
{
    const string EMPTY_STRING;
    ASSERT_NO_THROW(JsonObject::FromString(EMPTY_STRING));
}

TEST(JsonObjectTests, InvalidJsonTest)
{
    const auto INVALID_JSON_STRING = R"({"key" : })"s;
    ASSERT_ANY_THROW(JsonObject::FromString(INVALID_JSON_STRING));
}

TEST(JsonObjectTests, NullJsonTest)
{
    const auto TEST_JSON = R"([null])"s;
    auto json = JsonObject::FromString(TEST_JSON);

    ASSERT_FALSE(json.IsEmpty());
    ASSERT_TRUE(json[0].IsEmpty());
}

TEST(JsonObjectTests, DoubleJsonTest)
{
    const auto DOUBLE_JSON_STRING = R"({"key" : 123} [true, false])"s;
    ASSERT_ANY_THROW(JsonObject::FromString(DOUBLE_JSON_STRING));
}

TEST(JsonObjectTests, SimpleJsonTest)
{
    const auto TEST_JSON =
        R"({
                "firstName": "Ivan",
                "lastName" : "Ivanov",
                "address" : {
                    "streetAddress": "Moskovskaya street, 101",
                    "city" : "Leningrad",
                    "postalCode" : 101101
                },
                "phoneNumbers" : [
                    "812 123-1234",
                    "916 123-4567"
                ]
            })"s
        ;

    auto json = JsonObject::FromString(TEST_JSON);
    const auto firstName = json["firstName"].GetAsString();
    const auto streetAddress = json["address"]["streetAddress"].GetAsString();
    const auto postalCode = json["address"]["postalCode"].GetAsInt64();
    const auto secondNumber = json["phoneNumbers"][1].GetAsString();

    EXPECT_EQ("Ivan", firstName);
    EXPECT_EQ("Moskovskaya street, 101", streetAddress);
    EXPECT_EQ(101101, postalCode);
    EXPECT_EQ("916 123-4567", secondNumber);
}

TEST(JsonObjectTests, AddDataTest)
{
    JsonObject json;
    json["string data"] = "data";
    json["number data"] = 123;
    json["bool data"] = true;

    ASSERT_TRUE(json["string data"].IsString());
    ASSERT_TRUE(json["number data"].IsNumber());
    ASSERT_TRUE(json["bool data"].IsBool());

    const auto stringData = json["string data"].GetAsString();
    const auto numberData = json["number data"].GetAsDouble();
    const auto boolData = json["bool data"].GetAsBool();

    EXPECT_EQ("data", stringData);
    EXPECT_EQ(123, numberData);
    EXPECT_EQ(true, boolData);
}
