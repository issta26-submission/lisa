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
//<ID> 1615
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
    cJSON_AddItemToObject(root, "list", arr);
    cJSON *s0 = cJSON_CreateString("one");
    cJSON *s1 = cJSON_CreateString("two");
    cJSON *s2 = cJSON_CreateString("three");
    cJSON_AddItemToArray(arr, s0);
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);

    // step 2: Configure
    int original_size = cJSON_GetArraySize(arr);
    cJSON *size_num = cJSON_CreateNumber((double)original_size);
    cJSON_AddItemToObject(root, "original_size", size_num);

    // step 3: Operate & Validate
    cJSON *detached = cJSON_DetachItemFromArray(arr, 1);
    cJSON_InsertItemInArray(arr, 1, detached);
    cJSON *replacement = cJSON_CreateString("TWO");
    cJSON_ReplaceItemInArray(arr, 1, replacement);
    cJSON *prepend = cJSON_CreateString("zero");
    cJSON_InsertItemInArray(arr, 0, prepend);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    char *full = cJSON_Print(root);
    cJSON_free(full);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}