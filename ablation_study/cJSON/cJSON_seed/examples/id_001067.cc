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
//<ID> 1067
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
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *n3 = cJSON_CreateNumber(3.0);
    cJSON *false_item = cJSON_CreateFalse();

    // step 2: Configure
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, n3);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToObject(root, "flag", false_item);

    // step 3: Operate & Validate
    int size = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "count", (double)size);
    cJSON *item0 = cJSON_GetArrayItem(arr, 0);
    cJSON *item1 = cJSON_GetArrayItem(arr, 1);
    cJSON *item2 = cJSON_GetArrayItem(arr, 2);
    cJSON_bool isnum0 = cJSON_IsNumber(item0);
    cJSON_bool isnum1 = cJSON_IsNumber(item1);
    cJSON_bool isnum2 = cJSON_IsNumber(item2);
    cJSON_bool is_false = cJSON_IsFalse(false_item);
    double sum = cJSON_GetNumberValue(item0) + cJSON_GetNumberValue(item1) + cJSON_GetNumberValue(item2);
    cJSON *sumnum = cJSON_CreateNumber(sum);
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(arr, item1, sumnum);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}