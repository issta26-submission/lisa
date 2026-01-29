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
//<ID> 427
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = " { \"name\": \"example\", \"values\": [10, 20, 30], \"count\": 3 } ";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(raw_json, &parse_end, 1);

    // step 2: Configure
    cJSON *values_arr = cJSON_GetObjectItem(root, "values");
    cJSON_bool values_is_array = cJSON_IsArray(values_arr);
    cJSON_AddNumberToObject(root, "added", 99.0);
    cJSON *second_item = cJSON_GetArrayItem(values_arr, 1);
    double second_val = cJSON_GetNumberValue(second_item);
    cJSON *added_item = cJSON_GetObjectItem(root, "added");
    double added_val = cJSON_GetNumberValue(added_item);
    cJSON_AddNumberToObject(root, "sum_estimate", second_val + added_val);

    // step 3: Operate & Validate
    char status_buf[256];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "is_array=%d|second=%.0f|added=%.0f|sum_est=%.0f",
            (int)values_is_array, second_val, added_val, second_val + added_val);
    cJSON *status_str = cJSON_CreateString(status_buf);
    cJSON_AddItemToObject(root, "status", status_str);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_str = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapshot_str);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}