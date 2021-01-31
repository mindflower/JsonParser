#include <gtest/gtest.h>
#include <json/json_object.h>

using namespace std;
using namespace json;

TEST(JsonObjectTests, EmptyJsonTest)
{
    const wstring EMPTY_STRING;
    ASSERT_NO_THROW(JsonObject::FromString(EMPTY_STRING));
}

TEST(JsonObjectTests, InvalidJsonTest)
{
    const auto INVALID_JSON_STRING = LR"({"key" : })"s;
    ASSERT_ANY_THROW(JsonObject::FromString(INVALID_JSON_STRING));
}

TEST(JsonObjectTests, NullJsonTest)
{
    const auto TEST_JSON = LR"([null])"s;
    auto json = JsonObject::FromString(TEST_JSON);

    ASSERT_FALSE(json.IsEmpty());
    ASSERT_TRUE(json[0].IsEmpty());
}

TEST(JsonObjectTests, DoubleJsonTest)
{
    const auto DOUBLE_JSON_STRING = LR"({"key" : 123} [true, false])"s;
    ASSERT_ANY_THROW(JsonObject::FromString(DOUBLE_JSON_STRING));
}

TEST(JsonObjectTests, SimpleJsonTest)
{
    const auto TEST_JSON =
        LR"({
                "firstName": "Иван",
                "lastName" : "Иванов",
                "address" : {
                    "streetAddress": "Московское ш., 101, кв.101",
                    "city" : "Ленинград",
                    "postalCode" : 101101
                },
                "phoneNumbers" : [
                    "812 123-1234",
                    "916 123-4567"
                ]
            })"s
        ;

    auto json = JsonObject::FromString(TEST_JSON);
    const auto firstName = json[L"firstName"].GetAsString();
    const auto streetAddress = json[L"address"][L"streetAddress"].GetAsString();
    const auto postalCode = json[L"address"][L"postalCode"].GetAsInt64();
    const auto secondNumber = json[L"phoneNumbers"][1].GetAsString();

    EXPECT_EQ(L"Иван", firstName);
    EXPECT_EQ(L"Московское ш., 101, кв.101", streetAddress);
    EXPECT_EQ(101101, postalCode);
    EXPECT_EQ(L"916 123-4567", secondNumber);
}

TEST(JsonObjectTests, AddDataTest)
{
    JsonObject json;
    json[L"string data"] = L"data";
    json[L"number data"] = 123;
    json[L"bool data"] = true;

    ASSERT_TRUE(json[L"string data"].IsString());
    ASSERT_TRUE(json[L"number data"].IsNumber());
    ASSERT_TRUE(json[L"bool data"].IsBool());

    const auto stringData = json[L"string data"].GetAsString();
    const auto numberData = json[L"number data"].GetAsDobule();
    const auto boolData = json[L"bool data"].GetAsBool();

    EXPECT_EQ(L"data", stringData);
    EXPECT_EQ(123, numberData);
    EXPECT_EQ(true, boolData);
}
