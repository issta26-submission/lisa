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
//<ID> 422
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = " { \"arr\" : [1, 2, 3], \"base\" : 4.5 } ";
    char json_buf[128];
    memset(json_buf, 0, sizeof(json_buf));
    memcpy(json_buf, raw_json, strlen(raw_json) + 1);
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_buf, &parse_end, 1);

    // step 2: Configure
    cJSON *base_item = cJSON_GetObjectItem(root, "base");
    double base_val = cJSON_GetNumberValue(base_item);
    cJSON_AddNumberToObject(root, "computed", base_val * 2.0);
    cJSON *arr_item = cJSON_GetObjectItem(root, "arr");
    cJSON_bool arr_is_array = cJSON_IsArray(arr_item);
    cJSON_AddNumberToObject(root, "arr_is_array", (double)arr_is_array);

    // step 3: Operate & Validate
    cJSON *computed_item = cJSON_GetObjectItem(root, "computed");
    double computed_val = cJSON_GetNumberValue(computed_item);
    char status_buf[256];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "base=%.2f|computed=%.2f|arr_is_array=%d", base_val, computed_val, (int)arr_is_array);
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