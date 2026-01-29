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
//<ID> 1308
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
    cJSON *s1 = cJSON_CreateString("first");
    cJSON *s2 = cJSON_CreateString("second");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "list", arr);
    cJSON *f = cJSON_AddFalseToObject(root, "enabled");
    cJSON *replacement = cJSON_CreateString("second_replaced");
    cJSON_bool replaced = cJSON_ReplaceItemInArray(arr, 1, replacement);

    // step 2: Operate & Validate
    char *out = cJSON_PrintUnformatted(root);
    cJSON *list_obj = cJSON_GetObjectItem(root, "list");
    cJSON_bool is_invalid = cJSON_IsInvalid(list_obj);
    int count = cJSON_GetArraySize(arr);
    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    const char *first_val = cJSON_GetStringValue(first_item);
    size_t len = strlen(first_val);
    char *copy = (char *)cJSON_malloc(len + 1);
    memset(copy, 0, len + 1);
    memcpy(copy, first_val, len + 1);

    // step 3: Cleanup
    cJSON_free(out);
    cJSON_free(copy);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}