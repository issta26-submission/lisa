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
//<ID> 1066
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
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, n3);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToObject(root, "flag", flag);

    // step 2: Configure
    int size = cJSON_GetArraySize(arr);
    cJSON *size_num = cJSON_CreateNumber((double)size);
    cJSON_AddItemToObject(root, "size", size_num);

    // step 3: Operate & Validate
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    cJSON_bool was_number = cJSON_IsNumber(second);
    cJSON_bool was_false = cJSON_IsFalse(flag);
    cJSON *replacement = cJSON_CreateNumber(42.0);
    cJSON_ReplaceItemViaPointer(arr, second, replacement);
    cJSON *after = cJSON_GetArrayItem(arr, 1);
    cJSON_bool now_number = cJSON_IsNumber(after);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    (void)was_number;
    (void)was_false;
    (void)now_number;
    return 66; // API sequence test completed successfully
}