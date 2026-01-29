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
//<ID> 942
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(1.5);
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON *n3 = cJSON_CreateNumber(3.5);
    cJSON *btrue = cJSON_CreateTrue();
    cJSON *bfalse = cJSON_CreateFalse();
    cJSON *name = cJSON_CreateString("sensor-A");

    // step 2: Configure
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n3);
    cJSON_InsertItemInArray(arr, 1, n2);
    cJSON_AddItemToObject(config, "values", arr);
    cJSON_AddItemToObject(config, "enabled_true", btrue);
    cJSON_AddItemToObject(config, "enabled_false", bfalse);
    cJSON_AddItemToObject(config, "name", name);
    cJSON_AddItemToObject(root, "config", config);
    cJSON *dup_second = cJSON_Duplicate(n2, 1);
    cJSON_AddItemToObject(root, "second_copy", dup_second);

    // step 3: Operate & Validate
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);
    double second_val = cJSON_GetNumberValue(second_item);
    cJSON *enabled_item = cJSON_GetObjectItem(config, "enabled_true");
    cJSON_bool enabled_is_bool = cJSON_IsBool(enabled_item);
    cJSON_bool numbers_equal = cJSON_Compare(second_item, dup_second, 1);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    (void)second_val;
    (void)enabled_is_bool;
    (void)numbers_equal;
    return 66;
    // API sequence test completed successfully
}