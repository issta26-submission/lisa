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
//<ID> 280
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
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON *n2 = cJSON_CreateNumber(-3.14);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);

    // step 2: Configure
    cJSON *child = cJSON_CreateObject();
    cJSON_AddTrueToObject(child, "ok");
    cJSON_AddNumberToObject(child, "count", 2.0);
    cJSON_AddItemToObject(root, "child", child);
    cJSON *ref = cJSON_CreateArrayReference(child);
    cJSON_AddItemToArray(arr, ref);
    cJSON *b = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, b);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    cJSON *bool_item = cJSON_GetArrayItem(arr, 3);
    double first_val = cJSON_GetNumberValue(first);
    cJSON_AddNumberToObject(root, "first_value", first_val);
    cJSON_bool is_bool = cJSON_IsBool(bool_item);
    cJSON_AddBoolToObject(root, "bool_check", is_bool);
    char *dump = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(dump);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}