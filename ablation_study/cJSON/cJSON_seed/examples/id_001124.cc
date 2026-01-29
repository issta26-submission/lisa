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
//<ID> 1124
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
    cJSON *num0 = cJSON_CreateNumber(3.14);
    cJSON *num1 = cJSON_CreateNumber(2.71);
    const char *version = cJSON_Version();
    /* Use normal string creations instead of string-reference to avoid lifetime/double-free issues */
    cJSON *label_ref = cJSON_CreateString("initial_label");

    // step 2: Configure
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToObject(root, "label", label_ref);
    cJSON_AddStringToObject(root, "cjson_version", version);
    /* Duplicate the array rather than creating an array-reference to avoid ownership conflicts */
    cJSON *arr_ref = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "items_ref", arr_ref);

    // step 3: Operate & Validate
    cJSON *new_label_ref = cJSON_CreateString("replaced_label");
    cJSON_ReplaceItemInObjectCaseSensitive(root, "label", new_label_ref);
    cJSON *it0 = cJSON_GetArrayItem(arr, 0);
    cJSON *it1 = cJSON_GetArrayItem(arr, 1);
    double v0 = cJSON_GetNumberValue(it0);
    double v1 = cJSON_GetNumberValue(it1);
    double total = v0 + v1;
    cJSON_AddNumberToObject(root, "sum", total);
    cJSON *label_item = cJSON_GetObjectItem(root, "label");
    char *label_value = cJSON_GetStringValue(label_item);
    cJSON_AddStringToObject(root, "label_copy", label_value);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
}