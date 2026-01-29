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
//<ID> 1076
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"values\":[1,2,3],\"flag\":false,\"count\":1}";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, json_len);

    // step 2: Configure
    double extra_nums[] = {4.4, 5.5, 6.6};
    cJSON *doubles = cJSON_CreateDoubleArray(extra_nums, 3);
    cJSON_AddItemToObject(root, "doubles", doubles);
    cJSON *ok_flag = cJSON_AddTrueToObject(root, "ok");

    // step 3: Operate
    cJSON *new_count = cJSON_CreateNumber(42.0);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "count", new_count);
    cJSON *values = cJSON_GetObjectItem(root, "values");
    int values_count = cJSON_GetArraySize(values);
    cJSON *first_val_item = cJSON_GetArrayItem(values, 0);
    double first_val = cJSON_GetNumberValue(first_val_item);
    cJSON *summary = cJSON_CreateObject();
    cJSON_AddItemToObject(summary, "first_value", cJSON_CreateNumber(first_val));
    cJSON_AddItemToObject(summary, "values_count", cJSON_CreateNumber((double)values_count));
    cJSON_AddItemToObject(root, "summary", summary);

    // step 4: Validate & Cleanup
    char *printed = cJSON_Print(root);
    cJSON_free(printed);
    (void)ok_flag;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}