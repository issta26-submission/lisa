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
//<ID> 428
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char raw_json[] = " { \"values\": [10, 20, 30], \"offset\": 5 } ";
    char json_buf[128];
    memset(json_buf, 0, sizeof(json_buf));
    memcpy(json_buf, raw_json, strlen(raw_json) + 1);
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_buf, &parse_end, 1);

    // step 2: Configure
    cJSON *values = cJSON_GetObjectItem(root, "values");
    cJSON *offset = cJSON_GetObjectItem(root, "offset");
    cJSON_bool values_is_array = cJSON_IsArray(values);
    cJSON *first = cJSON_GetArrayItem(values, 0);
    cJSON *second = cJSON_GetArrayItem(values, 1);
    double first_val = cJSON_GetNumberValue(first);
    double second_val = cJSON_GetNumberValue(second);
    double offset_val = cJSON_GetNumberValue(offset);
    double computed_sum = first_val + second_val + offset_val;
    cJSON_AddNumberToObject(root, "computed_sum", computed_sum);
    cJSON_AddNumberToObject(root, "is_values_array", (double)values_is_array);

    // step 3: Operate & Validate
    cJSON *comp_item = cJSON_GetObjectItem(root, "computed_sum");
    cJSON *arrflag_item = cJSON_GetObjectItem(root, "is_values_array");
    double comp_val = cJSON_GetNumberValue(comp_item);
    double arrflag_val = cJSON_GetNumberValue(arrflag_item);
    (void)comp_val;
    (void)arrflag_val;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}