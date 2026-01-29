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
//<ID> 447
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
    cJSON *num1 = cJSON_CreateNumber(10.5);
    cJSON_AddItemToArray(values, num1);
    cJSON *num2 = cJSON_CreateNumber(20.25);
    cJSON_AddItemToArray(values, num2);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", flag_false);
    cJSON *nil_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "missing", nil_item);

    // step 2: Configure
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *threshold = cJSON_CreateNumber(5.0);
    cJSON_AddItemToObject(config, "threshold", threshold);

    // step 3: Operate & Validate
    cJSON *retrieved_values = cJSON_GetObjectItem(root, "values");
    cJSON *v0 = cJSON_GetArrayItem(retrieved_values, 0);
    cJSON *v1 = cJSON_GetArrayItem(retrieved_values, 1);
    double d0 = cJSON_GetNumberValue(v0);
    double d1 = cJSON_GetNumberValue(v1);
    cJSON *retrieved_threshold = cJSON_GetObjectItem(root, "config");
    cJSON *threshold_item = cJSON_GetObjectItem(retrieved_threshold, "threshold");
    double thr = cJSON_GetNumberValue(threshold_item);
    double total = d0 + d1 + thr;
    cJSON *maybe_null = cJSON_GetObjectItem(root, "missing");
    cJSON_bool is_null = cJSON_IsNull(maybe_null);
    char status_buf[192];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "sum=%.4f|isnull=%d", total, (int)is_null);
    cJSON_AddStringToObject(root, "status", status_buf);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_str = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapshot_str);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}