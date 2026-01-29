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
//<ID> 1068
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
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, n3);
    cJSON_AddItemToObject(root, "nums", arr);
    cJSON_AddItemToObject(root, "flag", flag_false);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateNumber(20.0);
    cJSON_ReplaceItemViaPointer(arr, n2, replacement);

    // step 3: Operate & Validate
    int count = cJSON_GetArraySize(arr);
    cJSON *item0 = cJSON_GetArrayItem(arr, 0);
    cJSON *item1 = cJSON_GetArrayItem(arr, 1);
    cJSON *item2 = cJSON_GetArrayItem(arr, 2);
    cJSON_bool item1_is_number = cJSON_IsNumber(item1);
    cJSON *flag_item = cJSON_GetObjectItem(root, "flag");
    cJSON_bool flag_is_false = cJSON_IsFalse(flag_item);
    double sum = cJSON_GetNumberValue(item0) + cJSON_GetNumberValue(item1) + cJSON_GetNumberValue(item2);
    cJSON *sum_num = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sum_num);
    cJSON *count_num = cJSON_CreateNumber((double)count);
    cJSON_AddItemToObject(root, "count", count_num);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}