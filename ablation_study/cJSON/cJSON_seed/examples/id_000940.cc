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
//<ID> 940
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
    cJSON *num1 = cJSON_CreateNumber(7.0);
    cJSON *btrue = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, btrue);
    cJSON_bool inserted = cJSON_InsertItemInArray(arr, 1, num1);
    (void)inserted;
    cJSON_AddItemToObject(root, "values", arr);

    // step 3: Operate & Validate
    cJSON *it0 = cJSON_GetArrayItem(arr, 0);
    double v0 = cJSON_GetNumberValue(it0);
    cJSON *it1 = cJSON_GetArrayItem(arr, 1);
    double v1 = cJSON_GetNumberValue(it1);
    cJSON *it2 = cJSON_GetArrayItem(arr, 2);
    cJSON_bool is_bool = cJSON_IsBool(it2);
    cJSON *dup1 = cJSON_Duplicate(it1, 1);
    cJSON_bool same = cJSON_Compare(it1, dup1, 1);
    cJSON *sum_item = cJSON_CreateNumber(v0 + v1);
    cJSON *bool_item = cJSON_CreateBool(is_bool);
    cJSON *cmp_item = cJSON_CreateBool(same);
    cJSON_AddItemToObject(root, "sum", sum_item);
    cJSON_AddItemToObject(root, "has_bool", bool_item);
    cJSON_AddItemToObject(root, "cmp_same", cmp_item);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(dup1);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}