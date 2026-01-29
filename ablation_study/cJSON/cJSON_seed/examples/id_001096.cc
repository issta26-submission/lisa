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
//<ID> 1096
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *info = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("deviceA");
    cJSON *status = cJSON_CreateString("old_status");
    cJSON *values = cJSON_CreateArray();
    cJSON *n0 = cJSON_CreateNumber(10.0);
    cJSON *n1 = cJSON_CreateNumber(20.0);
    cJSON *n2 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToObject(info, "name", name);
    cJSON_AddItemToObject(info, "status", status);
    cJSON_AddItemToArray(values, n0);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddItemToObject(root, "info", info);
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    cJSON *false_item = cJSON_CreateFalse();

    // step 3: Operate & Validate
    const char *name_val = cJSON_GetStringValue(cJSON_GetObjectItem(info, "name"));
    cJSON *name_copy = cJSON_CreateString(name_val);
    cJSON_AddItemToObject(root, "name_copy", name_copy);
    cJSON_ReplaceItemInObject(info, "status", false_item);
    cJSON *arr_item1 = cJSON_GetArrayItem(values, 1);
    cJSON *replacement_num = cJSON_CreateNumber(99.0);
    cJSON_ReplaceItemViaPointer(values, arr_item1, replacement_num);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}