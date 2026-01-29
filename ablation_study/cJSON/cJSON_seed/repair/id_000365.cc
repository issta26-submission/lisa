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
//<ID> 365
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_AddObjectToObject(root, "data");
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(data, "items", items);
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON *n3 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(items, n1);
    cJSON_AddItemToArray(items, n2);
    cJSON_AddItemToArray(items, n3);
    cJSON_AddNullToObject(data, "optional");

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *desc = cJSON_CreateString("three numbers");
    cJSON_AddItemToObject(meta, "description", desc);

    // step 3: Operate and Validate
    cJSON *got_items = cJSON_GetObjectItemCaseSensitive(data, "items");
    cJSON_bool is_array = cJSON_IsArray(got_items);
    cJSON *a0 = cJSON_GetArrayItem(got_items, 0);
    cJSON *a1 = cJSON_GetArrayItem(got_items, 1);
    cJSON *a2 = cJSON_GetArrayItem(got_items, 2);
    double sum = cJSON_GetNumberValue(a0) + cJSON_GetNumberValue(a1) + cJSON_GetNumberValue(a2);
    cJSON *sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sum_item);
    cJSON_AddItemToObject(meta, "is_array", cJSON_CreateBool(is_array));

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}