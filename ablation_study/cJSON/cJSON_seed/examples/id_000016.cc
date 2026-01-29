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
//<ID> 16
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize & Configure
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "value", 3.1415);
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *s1 = cJSON_CreateString("first");
    cJSON_AddItemToArray(arr, s1);
    cJSON *s2 = cJSON_CreateString("second");
    cJSON_AddItemToArray(arr, s2);

    // step 2: Operate
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_bool replaced_flag = cJSON_ReplaceItemInArray(arr, 0, replacement);
    cJSON_AddNumberToObject(root, "replaced_flag", (double)replaced_flag);

    // step 3: Validate
    cJSON *dup = cJSON_Duplicate(arr, 1);
    cJSON_bool equal_flag = cJSON_Compare(arr, dup, 1);
    cJSON_AddNumberToObject(root, "equal", (double)equal_flag);
    cJSON *nul = cJSON_CreateNull();
    cJSON_bool is_null_flag = cJSON_IsNull(nul);
    cJSON_AddNumberToObject(root, "is_null", (double)is_null_flag);

    // step 4: Cleanup
    cJSON_Delete(dup);
    cJSON_Delete(nul);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}