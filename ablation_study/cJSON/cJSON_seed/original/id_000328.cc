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
//<ID> 328
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
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(30.0));
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddStringToObject(child, "note", "initial");

    // step 2: Configure
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);
    cJSON *dup_second = cJSON_Duplicate(second_item, 0);
    cJSON_AddItemToObject(root, "duplicate_of_second", dup_second);
    cJSON_bool are_equal = cJSON_Compare(second_item, dup_second, 1);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddBoolToObject(meta, "second_equals_duplicate", are_equal);

    // step 3: Operate and Validate
    cJSON_DeleteItemFromArray(arr, 0);
    cJSON *new_first = cJSON_GetArrayItem(arr, 0);
    double new_first_val = cJSON_GetNumberValue(new_first);
    cJSON_AddNumberToObject(meta, "new_first_value", new_first_val);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON_bool prealloc_ok = cJSON_PrintPreallocated(root, buffer, 256, 0);
    cJSON_AddBoolToObject(meta, "printed_prealloc", prealloc_ok);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}