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
//<ID> 1080
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize & Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n0 = cJSON_CreateNumber(1.0);
    cJSON *n1 = cJSON_CreateNumber(2.0);
    cJSON *n2 = cJSON_CreateNumber(4.0);
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON *insert_num = cJSON_CreateNumber(3.0);
    cJSON_bool inserted = cJSON_InsertItemInArray(arr, 2, insert_num);

    // step 2: Operate
    cJSON *item_before = cJSON_GetArrayItem(arr, 2);
    cJSON_bool was_number = cJSON_IsNumber(item_before);
    cJSON *replacement = cJSON_CreateNumber(3.1415);
    cJSON_bool replaced = cJSON_ReplaceItemInArray(arr, 2, replacement);

    // step 3: Validate
    cJSON *item_after = cJSON_GetArrayItem(arr, 2);
    cJSON_bool is_number_after = cJSON_IsNumber(item_after);
    double numeric_value = cJSON_GetNumberValue(item_after);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    (void)inserted;
    (void)was_number;
    (void)replaced;
    (void)is_number_after;
    (void)numeric_value;
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}