// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 04.arrays.cpp
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
    JsonArray json_array = {"hello world!", false, true, nullptr};
    // let's try to add the desired element with the specified value..
    json_array.Add(1234.5670f);
    // let's try to get the desired element specifying its index..
    // in this case, we're not interested about the underlying type so
    // we only want to get the json-value back..
    // json_field is an instance of a JsonValue reference type so,
    // if we modify its value we're changing the original one (the one
    // stored within the json-array)..
    auto& json_field = json_array[0];
    json_field = "content changed via reference!";
    // let's try to get the desired element specifying its index..
    // in this case, we're interested about the underlying type so
    // we need to cast it to the expected value (please, note that if the
    // underlying type is not the expected one an exception will be triggered)..
    // json_field is an instance of a JsonNumber reference type so,
    // if we modify its value we're changing the original one (the one
    // stored within the json-array)..
    auto& json_field_specific = (JsonNumber&)json_array[4];
    json_field_specific = 765.4321f;
    // let's try to get the desired element specifying its inhdex..
    // in this case, we're interested about the underlying type so
    // we need to cast it to the expected value (please, note that if the
    // underlying type is not the expected one an exception will be triggered)..
    // json_field is an instance of a JsonNumber copied value type so,
    // if we modify its value we're NOT changing the original one (the one
    // stored within the json-array)..
    auto json_field_specific_copy = (JsonNumber&)json_array[4];
    json_field_specific_copy = 2020;
    // let's try now to change the original type of an stored json-value..
    auto& json_field_to_change = json_array[3];
    json_field_to_change = "another string value!";
    // let's try now to duplicate the original json-array content..
    auto json_array_copy = json_array;
    // let's try to add the copied json-array instance to the original one..
    json_array.Add(json_array_copy);
    // let's try to erase some elements..
    json_array.Erase(2);
    json_array.Erase(0,1); // from..to (both inclusive)
    // let's try now to dump (export to string) the entire json-array content..
    std::cout << json_array.ToString() << std::endl;
  } catch (const std::exception& exception) {
    // [error] -> while performing required operations!
    std::cout << exception.what() << std::endl;
    return -1;
  }
  return 0;
}
