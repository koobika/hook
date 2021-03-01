// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 03.objects.cpp
// -----------------------------------------------------------------------------
// Copyright (c) 2020 koobika corporation. All rights reserved.
// Author: Marcos Rojas (marcos.rojas@koobika.io)
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <iostream>

#include "structured/json/json_value.h"

using namespace koobika::hook::structured::json;

int main() {
  try {
    // straightforward declaration..
    JsonObject json_object = {{"field-a", "hello world!"},
                              {"field-b", false},
                              {"field-c", true},
                              {"field-d", nullptr}};
    // let's try to set the desired element with the specified value..
    json_object["field-e"] = 1234.5670f;
    // let's try to get the desired element specifying its key..
    // in this case, we're not interested about the underlying type so
    // we only want to get the json-value back..
    // json_field is an instance of a JsonValue reference type so, 
    // if we modify its value we're changing the original one (the one 
    // stored within the json-object)..
    auto& json_field = json_object["field-a"];
    json_field = "content changed via reference!";
    // let's try to get the desired element specifying its key..
    // in this case, we're interested about the underlying type so
    // we need to cast it to the expected value (please, note that if the 
    // underlying type is not the expected one an exception will be triggered)..
    // json_field is an instance of a JsonNumber reference type so,
    // if we modify its value we're changing the original one (the one
    // stored within the json-object)..
    auto& json_field_specific = (JsonNumber&)json_object["field-e"];
    json_field_specific = 765.4321f;
    // let's try to get the desired element specifying its key..
    // in this case, we're interested about the underlying type so
    // we need to cast it to the expected value (please, note that if the
    // underlying type is not the expected one an exception will be triggered)..
    // json_field is an instance of a JsonNumber copied value type so,
    // if we modify its value we're NOT changing the original one (the one
    // stored within the json-object)..
    auto json_field_specific_copy = (JsonNumber&)json_object["field-e"];
    json_field_specific_copy = 2020;
    // let's try now to change the original type of an stored json-value..
    auto& json_field_to_change = json_object["field-d"];
    json_field_to_change = "another string value!";
    // let's try now to duplicate the original json-object content..
    auto json_object_copy = json_object;
    // let's try to add the copied json-object instance to the original one..
    json_object["field-f"] = json_object_copy;
    // let's try to erase some elements..
    json_object.Erase("field-a");
    json_object.Erase({"field-b", "field-c", "field-d", "field-e"});
    // let's check for an object entry..
    auto json_field_exists = json_object.Exist("field-a");
    std::cout << "field-a does " << (json_field_exists ? "" : "NOT ")
              << "exist!" << std::endl;
    // let's try now to dump (export to string) the entire json-object content..
    std::cout << json_object.ToString() << std::endl;
  } catch (const std::exception& exception) {
    // [error] -> while performing required operations!
    std::cout << exception.what() << std::endl;
    return -1;
  }
  return 0;
}
