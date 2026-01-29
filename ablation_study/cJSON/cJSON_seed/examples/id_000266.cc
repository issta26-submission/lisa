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
//<ID> 266
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
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON *raw = cJSON_CreateRaw("raw_value");
    const char *ver = cJSON_Version();
    cJSON *ver_ref = cJSON_CreateStringReference(ver);

    // step 2: Configure
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, raw);
    cJSON_AddItemToArray(arr, ver_ref);
    int *ints = (int *)cJSON_malloc(sizeof(int) * 3);
    ints[0] = 7;
    ints[1] = 8;
    ints[2] = 9;
    cJSON *int_arr = cJSON_CreateIntArray(ints, 3);
    cJSON_AddItemToObject(root, "ints", int_arr);
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Operate & Validate
    cJSON *replacement = cJSON_CreateString("replaced_value");
    cJSON_bool replaced = cJSON_ReplaceItemInArray(arr, 1, replacement);
    (void)replaced;
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    cJSON_bool second_is_raw = cJSON_IsRaw(second);
    cJSON *raw_flag = cJSON_CreateNumber((double)second_is_raw);
    cJSON_AddItemToObject(root, "second_is_raw", raw_flag);
    cJSON *size_num = cJSON_CreateNumber((double)cJSON_GetArraySize(arr));
    cJSON_AddItemToObject(root, "array_size", size_num);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(ints);
    return 66; // API sequence test completed successfully
}