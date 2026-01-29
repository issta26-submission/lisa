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
//<ID> 13
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();

    // step 2: Configure
    cJSON *item_str = cJSON_CreateString("alpha");
    cJSON *item_num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(array, item_str);
    cJSON_AddItemToArray(array, item_num);
    cJSON_AddItemToObject(root, "items", array);
    cJSON *count_item = cJSON_AddNumberToObject(root, "count", 2.0);

    // step 3: Operate & Validate
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToArray(array, null_item);
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_bool replaced_ok = cJSON_ReplaceItemInArray(array, 1, replacement);
    (void)replaced_ok;
    cJSON *dup_array = cJSON_Duplicate(array, 1);
    cJSON_bool equal_arrays = cJSON_Compare(array, dup_array, 1);
    (void)equal_arrays;
    cJSON_bool is_null = cJSON_IsNull(null_item);
    (void)is_null;
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    if (out) { cJSON_free(out); }
    cJSON_Delete(dup_array);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)count_item;
    return 66;
}