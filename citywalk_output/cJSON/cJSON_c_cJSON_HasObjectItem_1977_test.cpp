// C++11 unit test suite for cJSON_HasObjectItem (no GTest, lightweight harness)

#include <locale.h>
#include <math.h>
#include <cJSON.h>
#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <float.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <limits.h>


// Include the C library header with C linkage
extern "C" {
}

// Simple lightweight test harness (non-terminating, prints results)
static int g_total_tests = 0;
static int g_failed_tests = 0;

static void report_result(const std::string& test_name, bool passed) {
    ++g_total_tests;
    if (!passed) {
        ++g_failed_tests;
        std::cerr << "[FAIL] " << test_name << std::endl;
    } else {
        std::cout << "[PASS] " << test_name << std::endl;
    }
}

// Test 1: Basic true case
// - Create an object with a key "name" and a string value.
// - Verify that cJSON_HasObjectItem(obj, "name") returns true (1).
// - This covers the true branch when the key exists.
static bool Test_HasObjectItem_BasicTrue() {
    cJSON *obj = cJSON_CreateObject();
    if (obj == nullptr) {
        report_result("Test_HasObjectItem_BasicTrue: obj creation failed", false);
        return false;
    }

    // Add a string item with key "name"
    if (cJSON_AddStringToObject(obj, "name", "Alice") == nullptr) {
        cJSON_Delete(obj);
        report_result("Test_HasObjectItem_BasicTrue: add item failed", false);
        return false;
    }

    int has = cJSON_HasObjectItem(obj, "name");
    cJSON_Delete(obj);

    bool passed = (has != 0);
    report_result("Test_HasObjectItem_BasicTrue", passed);
    return passed;
}

// Test 2: Case-insensitive key lookup (true case)
// - Create an object with key "Name" (capital N) and search for "name" (lowercase).
// - Verify that cJSON_HasObjectItem(obj, "name") returns true, demonstrating case-insensitive search
//   by the public API cJSON_GetObjectItem (and thus HasObjectItem).
static bool Test_HasObjectItem_CaseInsensitiveTrue() {
    cJSON *obj = cJSON_CreateObject();
    if (obj == nullptr) {
        report_result("Test_HasObjectItem_CaseInsensitiveTrue: obj creation failed", false);
        return false;
    }

    if (cJSON_AddStringToObject(obj, "Name", "Bob") == nullptr) {
        cJSON_Delete(obj);
        report_result("Test_HasObjectItem_CaseInsensitiveTrue: add item failed", false);
        return false;
    }

    int has = cJSON_HasObjectItem(obj, "name"); // different case
    cJSON_Delete(obj);

    bool passed = (has != 0);
    report_result("Test_HasObjectItem_CaseInsensitiveTrue", passed);
    return passed;
}

// Test 3: Missing key should return false
// - Create an object with a different key and verify that a non-existent key yields false.
static bool Test_HasObjectItem_MissingFalse() {
    cJSON *obj = cJSON_CreateObject();
    if (obj == nullptr) {
        report_result("Test_HasObjectItem_MissingFalse: obj creation failed", false);
        return false;
    }

    if (cJSON_AddStringToObject(obj, "existing", "value") == nullptr) {
        cJSON_Delete(obj);
        report_result("Test_HasObjectItem_MissingFalse: add item failed", false);
        return false;
    }

    int has = cJSON_HasObjectItem(obj, "missing");
    cJSON_Delete(obj);

    bool passed = (has == 0);
    report_result("Test_HasObjectItem_MissingFalse", passed);
    return passed;
}

// Test 4: Empty object should return false for any key
// - Create an empty object and verify that querying any key yields false.
static bool Test_HasObjectItem_EmptyObjectFalse() {
    cJSON *obj = cJSON_CreateObject();
    if (obj == nullptr) {
        report_result("Test_HasObjectItem_EmptyObjectFalse: obj creation failed", false);
        return false;
    }

    int has = cJSON_HasObjectItem(obj, "anything");
    cJSON_Delete(obj);

    bool passed = (has == 0);
    report_result("Test_HasObjectItem_EmptyObjectFalse", passed);
    return passed;
}

int main() {
    // Run all tests
    bool t1 = Test_HasObjectItem_BasicTrue();
    bool t2 = Test_HasObjectItem_CaseInsensitiveTrue();
    bool t3 = Test_HasObjectItem_MissingFalse();
    bool t4 = Test_HasObjectItem_EmptyObjectFalse();

    // Final summary
    std::cout << "Total tests run: " << g_total_tests << "\n";
    std::cout << "Total failures: " << g_failed_tests << "\n";

    // Return success only if all tests passed
    if (g_failed_tests == 0) {
        return 0;
    } else {
        return 1;
    }
}