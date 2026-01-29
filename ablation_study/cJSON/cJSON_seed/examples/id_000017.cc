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
//<ID> 17
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
    cJSON_AddItemToObject(root, "items", arr);

    // step 2: Configure
    cJSON *versionItem = cJSON_AddNumberToObject(root, "version", 1.234);
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(arr, n2);
    cJSON *s1 = cJSON_CreateString("initial");
    cJSON_AddItemToArray(arr, s1);
    cJSON *nullItem = cJSON_CreateNull();
    cJSON_AddItemToArray(arr, nullItem);

    // step 3: Operate & Validate
    cJSON_bool was_null = cJSON_IsNull(nullItem);
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_bool replaced = cJSON_ReplaceItemInArray(arr, 1, replacement);
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, dup, 1);
    cJSON_SetNumberHelper(versionItem, 200.0 + (double)was_null);
    cJSON_AddNumberToObject(root, "checks", (double)replaced + 2.0 * (double)equal);

    // step 4: Cleanup
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}