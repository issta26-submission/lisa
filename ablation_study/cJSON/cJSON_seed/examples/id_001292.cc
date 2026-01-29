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
//<ID> 1292
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
    cJSON *s1 = cJSON_CreateString("hello");
    cJSON *s2 = cJSON_CreateString("world");
    cJSON *flag = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON *dup = cJSON_Duplicate(arr, 1);
    cJSON_AddItemReferenceToObject(root, "items_ref", dup);

    // step 3: Operate & Validate
    cJSON_bool is_true = cJSON_IsTrue(flag);
    int size = cJSON_GetArraySize(arr);
    int total = size + (int)is_true;
    cJSON_AddNumberToObject(root, "total", (double)total);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    const char *val = cJSON_GetStringValue(first);
    size_t len = strlen(val);
    char *copy = (char *)cJSON_malloc(len + 1);
    memcpy(copy, val, len + 1);

    // step 4: Cleanup
    cJSON_free(copy);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}