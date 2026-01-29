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
//<ID> 1130
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
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(3.0));
    const char *ref_str = "constant_ref";
    cJSON *ref_item = cJSON_CreateStringReference(ref_str);
    cJSON_AddItemToObject(root, "ref", ref_item);
    cJSON_AddNullToObject(root, "nothing");
    cJSON_AddItemToObject(root, "enabled", cJSON_CreateTrue());

    // step 2: Configure
    cJSON *replacement = cJSON_CreateNumber(42.0);
    cJSON_ReplaceItemInArray(arr, 1, replacement);

    // step 3: Operate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    cJSON *third = cJSON_GetArrayItem(arr, 2);
    double v1 = cJSON_GetNumberValue(first);
    double v2 = cJSON_GetNumberValue(second);
    double v3 = cJSON_GetNumberValue(third);
    double sum = v1 + v2 + v3;
    cJSON_AddNumberToObject(root, "sum", sum);
    cJSON *dup_arr = cJSON_Duplicate(arr, 1);
    cJSON_bool equal = cJSON_Compare(arr, dup_arr, 1);
    char *flat = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON_PrintPreallocated(root, buffer, 256, 1);
    cJSON_free(buffer);
    cJSON_free(flat);
    cJSON_Delete(dup_arr);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}