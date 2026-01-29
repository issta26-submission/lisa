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
//<ID> 1614
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
    cJSON *s1 = cJSON_CreateString("one");
    cJSON *s2 = cJSON_CreateString("two");
    cJSON *s3 = cJSON_CreateString("three");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToArray(arr, s3);

    // step 2: Configure
    int initial_size = cJSON_GetArraySize(arr);
    cJSON *size_num = cJSON_CreateNumber((double)initial_size);
    cJSON_AddItemToObject(root, "initial_size", size_num);

    // step 3: Operate & Validate
    cJSON *ins = cJSON_CreateString("inserted");
    cJSON_InsertItemInArray(arr, 1, ins);
    cJSON *repl = cJSON_CreateString("replaced");
    cJSON_ReplaceItemInArray(arr, 2, repl);
    cJSON *detached = cJSON_DetachItemFromArray(arr, 3);
    cJSON *container = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "detached_container", container);
    cJSON_AddItemToArray(container, detached);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    const char *first_val = cJSON_GetStringValue(first);
    cJSON *first_copy = cJSON_CreateString(first_val ? first_val : "");
    cJSON_AddItemToObject(root, "first_copy", first_copy);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Cleanup
    char *full = cJSON_Print(root);
    cJSON_free(full);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}