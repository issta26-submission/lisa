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
//<ID> 1612
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
    cJSON *s0 = cJSON_CreateString("one");
    cJSON *s1 = cJSON_CreateString("two");
    cJSON *s2 = cJSON_CreateString("three");
    cJSON_AddItemToArray(arr, s0);
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);

    // step 2: Configure
    int initial_size = cJSON_GetArraySize(arr);
    cJSON *size_num = cJSON_CreateNumber((double)initial_size);
    cJSON_AddItemToObject(root, "items_count", size_num);

    // step 3: Operate & Validate
    cJSON *detached = cJSON_DetachItemFromArray(arr, 1);
    cJSON_Delete(detached);
    cJSON *insert_item = cJSON_CreateString("inserted");
    cJSON_InsertItemInArray(arr, 1, insert_item);
    cJSON *replace_item = cJSON_CreateString("replaced");
    cJSON_ReplaceItemInArray(arr, 2, replace_item);
    char *unformatted = cJSON_PrintUnformatted(root);
    cJSON_free(unformatted);

    // step 4: Cleanup
    char *full = cJSON_Print(root);
    cJSON_free(full);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}