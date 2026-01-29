#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 1065
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *n3 = cJSON_CreateNumber(3.0);
    cJSON *false_item = cJSON_CreateFalse();

    // step 2: Configure
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, n3);
    cJSON_AddItemToObject(root, "nums", arr);
    cJSON_AddItemToObject(root, "ok", false_item);
    cJSON_AddStringToObject(root, "label", "initial");

    // step 3: Operate & Validate
    int size = cJSON_GetArraySize(arr);
    cJSON *item0 = cJSON_GetArrayItem(arr, 0);
    cJSON *item1 = cJSON_GetArrayItem(arr, 1);
    cJSON *item2 = cJSON_GetArrayItem(arr, 2);
    cJSON_bool is0num = cJSON_IsNumber(item0);
    cJSON_bool is1num = cJSON_IsNumber(item1);
    cJSON_bool wasFalse = cJSON_IsFalse(false_item);
    cJSON *replacement = cJSON_CreateNumber(42.0);
    cJSON_ReplaceItemViaPointer(arr, item1, replacement);
    cJSON *newItem1 = cJSON_GetArrayItem(arr, 1);
    double v0 = cJSON_GetNumberValue(item0);
    double v1 = cJSON_GetNumberValue(newItem1);
    double v2 = cJSON_GetNumberValue(item2);
    cJSON *sum = cJSON_CreateNumber(v0 + v1 + v2);
    cJSON_AddItemToObject(root, "sum", sum);
    cJSON *countnum = cJSON_CreateNumber((double)size);
    cJSON_AddItemToObject(root, "count", countnum);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}