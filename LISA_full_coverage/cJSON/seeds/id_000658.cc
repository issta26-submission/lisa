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
//<ID> 658
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize & Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON *num = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToArray(arr, num);
    cJSON *sref = cJSON_CreateStringReference("hello");
    cJSON_AddItemToArray(arr, sref);
    cJSON_AddStringToObject(root, "greeting", "world");

    // step 2: Operate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON_bool first_is_number = cJSON_IsNumber(first);
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    cJSON_bool second_is_invalid = cJSON_IsInvalid(second);
    cJSON *detached = cJSON_DetachItemViaPointer(arr, first);
    cJSON_AddItemToObject(root, "detached_number", detached);

    // step 3: Validate
    cJSON_AddBoolToObject(root, "first_was_number", first_is_number);
    cJSON_AddBoolToObject(root, "second_was_invalid", second_is_invalid);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}