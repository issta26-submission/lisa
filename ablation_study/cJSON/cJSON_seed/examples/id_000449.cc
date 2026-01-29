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
//<ID> 449
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
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    cJSON *num1 = cJSON_CreateNumber(10.5);
    cJSON_AddItemToArray(values, num1);
    cJSON *num2 = cJSON_CreateNumber(20.25);
    cJSON_AddItemToArray(values, num2);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(values, false_item);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToArray(values, null_item);

    // step 3: Operate & Validate
    cJSON *retrieved_values = cJSON_GetObjectItem(root, "values");
    double v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(retrieved_values, 0));
    double v2 = cJSON_GetNumberValue(cJSON_GetArrayItem(retrieved_values, 1));
    double total = v1 + v2;
    cJSON_bool null_is_null = cJSON_IsNull(cJSON_GetArrayItem(retrieved_values, 3));
    char status_buf[128];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "sum=%.2f|null=%d", total, (int)null_is_null);
    cJSON_AddStringToObject(root, "status", status_buf);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_str = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapshot_str);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}