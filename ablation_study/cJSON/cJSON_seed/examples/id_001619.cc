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
//<ID> 1619
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
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON *s3 = cJSON_CreateString("gamma");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToArray(arr, s3);

    // step 2: Configure
    int initial_size = cJSON_GetArraySize(arr);
    cJSON *size_num = cJSON_CreateNumber((double)initial_size);
    cJSON_AddItemToObject(root, "initial_size", size_num);

    // step 3: Operate & Validate
    cJSON *detached = cJSON_DetachItemFromArray(arr, 1);
    cJSON_InsertItemInArray(arr, 2, detached);
    cJSON *replacement = cJSON_CreateString("delta");
    cJSON_ReplaceItemInArray(arr, 1, replacement);
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);
    const char *second_val = cJSON_GetStringValue(second_item);
    cJSON *second_copy = cJSON_CreateString(second_val);
    cJSON_AddItemToObject(root, "second_copy", second_copy);
    char *unformatted = cJSON_PrintUnformatted(root);
    cJSON_free(unformatted);

    // step 4: Cleanup
    char *full = cJSON_Print(root);
    cJSON_free(full);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}