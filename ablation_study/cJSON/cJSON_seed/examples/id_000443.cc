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
//<ID> 443
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = "{\"metrics\":{\"values\":[]},\"status\":null}";
    cJSON *root = cJSON_Parse(raw_json);
    cJSON *metrics = cJSON_GetObjectItem(root, "metrics");
    cJSON *values = cJSON_GetObjectItem(metrics, "values");

    // step 2: Configure
    cJSON *num_a = cJSON_CreateNumber(3.14);
    cJSON *num_b = cJSON_CreateNumber(2.71);
    cJSON_AddItemToArray(values, num_a);
    cJSON_AddItemToArray(values, num_b);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToArray(values, flag_false);

    // step 3: Operate & Validate
    cJSON *status_item = cJSON_GetObjectItem(root, "status");
    cJSON_bool status_is_null = cJSON_IsNull(status_item);
    cJSON *values_ref = cJSON_GetObjectItem(cJSON_GetObjectItem(root, "metrics"), "values");
    double v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(values_ref, 0));
    double v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(values_ref, 1));
    double total = v0 + v1;
    cJSON *avg_item = cJSON_CreateNumber(total / 2.0);
    cJSON_AddItemToObject(root, "average", avg_item);

    // step 4: Cleanup
    char status_buf[128];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "status_null=%d|sum=%.4f", (int)status_is_null, total);
    cJSON_AddStringToObject(root, "report", status_buf);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_str = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapshot_str);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}