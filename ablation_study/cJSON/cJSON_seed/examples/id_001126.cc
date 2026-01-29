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
//<ID> 1126
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num0 = cJSON_CreateNumber(1.0);
    cJSON *num1 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON *label_ref = cJSON_CreateStringReference("example_label");
    cJSON_AddItemToObject(root, "label", label_ref);
    cJSON_AddItemToObject(root, "values", arr);
    const char *ver = cJSON_Version();
    cJSON *ver_ref = cJSON_CreateStringReference(ver);
    cJSON_AddItemToObject(root, "version", ver_ref);
    cJSON *old_str = cJSON_CreateString("old_value");
    cJSON_AddItemToObject(root, "replace_target", old_str);
    cJSON *arr_dup = cJSON_Duplicate(arr, 1);
    cJSON *arr_ref = cJSON_CreateArrayReference(arr_dup);
    cJSON_bool replaced = cJSON_ReplaceItemInObjectCaseSensitive(root, "replace_target", arr_ref);
    (void)replaced;
    int size = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "values_count", (double)size);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
}