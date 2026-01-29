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
//<ID> 420
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = "{\"name\":\"example\",\"values\":[1,2,3],\"count\":3}";
    char json_buf[128];
    memset(json_buf, 0, sizeof(json_buf));
    memcpy(json_buf, raw_json, strlen(raw_json) + 1);
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_buf, &parse_end, 1);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddNumberToObject(meta, "generated_at", 20260115.0);
    cJSON_AddNumberToObject(meta, "version", 1.0);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *more_values = cJSON_CreateArray();
    cJSON_AddItemToArray(more_values, cJSON_CreateNumber(4.0));
    cJSON_AddItemToArray(more_values, cJSON_CreateNumber(5.0));
    cJSON_AddItemToObject(root, "more_values", more_values);

    // step 3: Operate & Validate
    cJSON *values_item = cJSON_GetObjectItem(root, "values");
    cJSON_bool values_are_array = cJSON_IsArray(values_item);
    cJSON *first_val_item = cJSON_GetArrayItem(values_item, 0);
    double first_val = cJSON_GetNumberValue(first_val_item);
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    double computed_sum = first_val + count_val;
    cJSON_AddNumberToObject(root, "computed_sum", computed_sum);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}