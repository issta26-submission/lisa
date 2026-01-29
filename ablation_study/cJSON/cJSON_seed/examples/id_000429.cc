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
//<ID> 429
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = "{ \"data\": [10, 20], \"value\": 7 }";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(raw_json, &parse_end, 1);

    // step 2: Configure
    cJSON *added_num_item = cJSON_AddNumberToObject(root, "added", 3.1415);

    // step 3: Operate & Validate
    cJSON *data_item = cJSON_GetObjectItem(root, "data");
    cJSON_bool data_is_array = cJSON_IsArray(data_item);
    double value_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "value"));
    double added_val = cJSON_GetNumberValue(added_num_item);
    double total = value_val + added_val;
    char status_buf[192];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "array=%d|value=%.2f|added=%.4f|total=%.4f", (int)data_is_array, value_val, added_val, total);
    cJSON_AddStringToObject(root, "status", status_buf);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}