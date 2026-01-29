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
//<ID> 1060
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
    cJSON *n0 = cJSON_CreateNumber(1.0);
    cJSON *n1 = cJSON_CreateNumber(2.0);
    cJSON *n2 = cJSON_CreateNumber(3.0);
    cJSON *f = cJSON_CreateFalse();

    // step 2: Configure
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, f);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddStringToObject(root, "label", "example");

    // step 3: Operate & Validate
    int count = cJSON_GetArraySize(arr);
    cJSON *it0 = cJSON_GetArrayItem(arr, 0);
    cJSON *it1 = cJSON_GetArrayItem(arr, 1);
    cJSON *it3 = cJSON_GetArrayItem(arr, 3);
    cJSON_bool it0_is_num = cJSON_IsNumber(it0);
    cJSON_bool it3_is_false = cJSON_IsFalse(it3);
    cJSON *replacement = cJSON_CreateNumber(42.0);
    cJSON_ReplaceItemViaPointer(arr, it1, replacement);
    cJSON *it1b = cJSON_GetArrayItem(arr, 1);
    cJSON_bool it1b_is_num = cJSON_IsNumber(it1b);
    double v0 = cJSON_GetNumberValue(it0);
    double v1b = cJSON_GetNumberValue(it1b);
    double v2 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 2));
    double sum = v0 + v1b + v2;
    int meta = (int)(it0_is_num + it1b_is_num + it3_is_false) + count;
    cJSON_AddNumberToObject(root, "sum", sum);
    cJSON_AddNumberToObject(root, "meta", (double)meta);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}