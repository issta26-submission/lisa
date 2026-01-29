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
//<ID> 412
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
    cJSON *n0 = cJSON_CreateNumber(10.0);
    cJSON *s1 = cJSON_CreateString("hello");
    cJSON *n2 = cJSON_CreateNumber(3.14);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToArray(array, n0);
    cJSON_AddItemToArray(array, s1);
    cJSON_AddItemToArray(array, n2);
    cJSON_AddItemToArray(array, maybe_null);
    cJSON_AddItemToObject(root, "data", array);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_ReplaceItemInArray(array, 1, replacement);
    cJSON *extra = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(array, extra);

    // step 3: Operate & Validate
    int count = cJSON_GetArraySize(array);
    cJSON_bool has_data = cJSON_HasObjectItem(root, "data");
    cJSON *third_item = cJSON_GetArrayItem(array, 2);
    cJSON_bool third_is_null = cJSON_IsNull(third_item);
    const char *repl_val = cJSON_GetStringValue(cJSON_GetArrayItem(array, 1));
    char status[192];
    memset(status, 0, sizeof(status));
    sprintf(status, "count=%d|has_data=%d|third_null=%d|replaced=%s", count, (int)has_data, (int)third_is_null, repl_val ? repl_val : "NULL");
    cJSON *status_str = cJSON_CreateString(status);
    cJSON_AddItemToObject(root, "status", status_str);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_str = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapshot_str);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}