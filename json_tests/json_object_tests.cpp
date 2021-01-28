#include <gtest/gtest.h>
#include <json/json_object.h>

using namespace std;
using namespace json;

TEST(JsonObjectTests, EmptyJsonTest)
{
    const wstring EMPTY_STRING;
    auto json = JsonObject::FromString(EMPTY_STRING);

    auto& data = json[L"Data"].GetAsString() = L"MyData";
    data = L"MyData";

    EXPECT_EQ(L"MyData", json[L"Data"].GetAsString());
}

TEST(JsonObjectTests, SimpleJsonTest)
{
    const auto TEST_JSON =
        LR"({
                "firstName": "����",
                "lastName" : "������",
                "address" : {
                    "streetAddress": "���������� �., 101, ��.101",
                    "city" : "���������",
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

    EXPECT_EQ(L"����", firstName);
    EXPECT_EQ(L"���������� �., 101, ��.101", streetAddress);
    EXPECT_EQ(L"916 123-4567", secondNumber);
}
