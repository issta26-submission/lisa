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
//<ID> 398
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
    cJSON *s1 = cJSON_CreateString("one");
    cJSON *s2 = cJSON_CreateString("two");
    cJSON_InsertItemInArray(arr, 0, s1);
    cJSON_InsertItemInArray(arr, 1, s2);
    cJSON_AddItemToObject(root, "list", arr);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag);

    // step 2: Configure
    cJSON *temp = cJSON_CreateString("temp");
    char *old_val = cJSON_SetValuestring(temp, "modified");
    cJSON_AddItemToArray(arr, temp);

    // step 3: Operate
    cJSON *detached = cJSON_DetachItemFromArray(arr, 1);
    cJSON_InsertItemInArray(arr, 0, detached);

    // step 4: Validate and Cleanup
    int sz = cJSON_GetArraySize(arr);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    const char *first_str = cJSON_GetStringValue(first);
    (void)old_val;
    (void)sz;
    (void)first_str;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}