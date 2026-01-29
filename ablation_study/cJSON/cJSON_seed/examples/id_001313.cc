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
//<ID> 1313
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *s = cJSON_CreateString("alpha");
    cJSON *b = cJSON_CreateTrue();
    cJSON *sref = cJSON_CreateStringReference(cJSON_GetStringValue(s));

    // step 2: Configure
    cJSON_AddItemToArray(arr, s);
    cJSON_AddItemToArray(arr, cJSON_CreateString("beta"));
    cJSON_AddItemToObject(child, "values", arr);
    cJSON_AddItemToObject(child, "flag", b);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(root, "sref", sref);
    cJSON_AddStringToObject(root, "label", "root_label");

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    const char *first_val = cJSON_GetStringValue(first);
    size_t len = strlen(first_val);
    char *copy = (char *)cJSON_malloc(len + 1);
    memset(copy, 0, len + 1);
    memcpy(copy, first_val, len + 1);
    cJSON_bool flag_is_bool = cJSON_IsBool(b);
    cJSON *num = cJSON_CreateNumber((double)flag_is_bool);
    cJSON_AddItemToObject(child, "flag_num", num);
    cJSON *dup_child = cJSON_Duplicate(child, 1);
    cJSON_DeleteItemFromObject(root, "sref");

    // step 4: Cleanup
    cJSON_free(copy);
    cJSON_Delete(dup_child);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}