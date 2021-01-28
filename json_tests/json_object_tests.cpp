#include <gtest/gtest.h>
#include <json/json_object.h>

using namespace std;
using namespace json;

TEST(JsonObjectTests, EmptyJsonTest)
{
    const wstring EMPTY_STRING;
    ASSERT_NO_THROW(JsonObject::FromString(EMPTY_STRING));
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
    const auto& firstName = json[L"firstName"].GetAsString();
    const auto& streetAddress = json[L"address"][L"streetAddress"].GetAsString();
    const auto& secondNumber = json[L"phoneNumbers"][1].GetAsString();

    EXPECT_EQ(L"Иван", firstName);
    EXPECT_EQ(L"Московское ш., 101, кв.101", streetAddress);
    EXPECT_EQ(L"916 123-4567", secondNumber);
}

TEST(JsonObjectTests, InvalidJsonTest)
{
    const auto INVALID_JSON_STRING = LR"({"key" : })"s;
    ASSERT_ANY_THROW(JsonObject::FromString(INVALID_JSON_STRING));
}

TEST(JsonObjectTests, DoubleJsonTest)
{
    const auto DOUBLE_JSON_STRING = LR"({"key" : 123} [true, false])"s;
    ASSERT_ANY_THROW(JsonObject::FromString(DOUBLE_JSON_STRING));
}

TEST(JsonObjectTests, AddDataTest)
{
    JsonObject json;
    auto& data = json[L"Data"].GetAsString();
    data = L"MyData";
    EXPECT_EQ(L"MyData", json[L"Data"].GetAsString());
}
