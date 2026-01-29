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
//<ID> 650
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_AddArrayToObject(root, "numbers");

    // step 2: Configure
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *num1 = cJSON_CreateNumber(3.14);
    cJSON *num2 = cJSON_CreateNumber(42.0);
    cJSON *sref = cJSON_CreateStringReference("example");
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToArray(arr, sref);

    // step 3: Operate
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    cJSON_bool second_is_number = cJSON_IsNumber(second);
    cJSON_AddNumberToObject(meta, "second_is_number", (double)second_is_number);
    cJSON *detached = cJSON_DetachItemViaPointer(arr, second);
    cJSON_AddItemToObject(root, "detached_number", detached);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON_bool first_is_invalid = cJSON_IsInvalid(first);
    cJSON_AddBoolToObject(meta, "first_is_invalid", first_is_invalid);

    // step 4: Validate & Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}