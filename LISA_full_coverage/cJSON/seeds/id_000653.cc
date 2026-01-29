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
//<ID> 653
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_AddArrayToObject(root, "arr");
    cJSON *sref = cJSON_CreateStringReference("greeting");
    cJSON_AddItemToArray(arr, sref);
    cJSON *num = cJSON_CreateNumber(123.0);
    cJSON_AddItemToArray(arr, num);

    // step 2: Operate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    cJSON_bool second_is_number = cJSON_IsNumber(second);
    cJSON_bool first_is_invalid = cJSON_IsInvalid(first);
    cJSON *detached_second = cJSON_DetachItemViaPointer(arr, second);

    // step 3: Validate
    cJSON_AddBoolToObject(root, "second_was_number", second_is_number);
    cJSON_AddBoolToObject(root, "first_was_invalid", first_is_invalid);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(detached_second);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}