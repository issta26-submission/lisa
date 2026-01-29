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
//<ID> 426
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = " { \"values\" : [ 1, 2, 3 ], \"base\" : 10 } ";
    char json_buf[256];
    memset(json_buf, 0, sizeof(json_buf));
    memcpy(json_buf, raw_json, strlen(raw_json) + 1);
    cJSON_Minify(json_buf);
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_buf, &parse_end, 1);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "added_value", 42.5);
    cJSON *values_arr = cJSON_GetObjectItem(root, "values");
    cJSON_bool arr_is_array = cJSON_IsArray(values_arr);

    // step 3: Operate & Validate
    cJSON *first_item = cJSON_GetArrayItem(values_arr, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON *base_item = cJSON_GetObjectItem(root, "base");
    double base_val = cJSON_GetNumberValue(base_item);
    double sum = first_val + base_val;
    cJSON_AddNumberToObject(root, "sum", sum);
    char status_buf[192];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "arr=%d|first=%.0f|base=%.0f|sum=%.0f", (int)arr_is_array, first_val, base_val, sum);
    cJSON *status = cJSON_CreateString(status_buf);
    cJSON_AddItemToObject(root, "status", status);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_str = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapshot_str);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}