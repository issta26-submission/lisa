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
//<ID> 413
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *n3 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddItemToArray(values, n3);
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToArray(values, null_item);
    cJSON *replacement = cJSON_CreateNumber(42.0);
    cJSON_bool replace_result = cJSON_ReplaceItemInArray(values, 1, replacement);

    // step 3: Operate & Validate
    int size = cJSON_GetArraySize(values);
    cJSON_bool has_values = cJSON_HasObjectItem(root, "values");
    cJSON *second_item = cJSON_GetArrayItem(values, 1);
    cJSON_bool second_is_null = cJSON_IsNull(second_item);
    cJSON_bool null_check = cJSON_IsNull(null_item);
    char status_buf[192];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "size=%d|has=%d|second_null=%d|null_item=%d|replaced=%d",
            size, (int)has_values, (int)second_is_null, (int)null_check, (int)replace_result);
    cJSON *status_str = cJSON_CreateString(status_buf);
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