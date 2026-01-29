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
//<ID> 425
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = " { \"values\": [ 10, 20, 30 ], \"meta\": { \"scale\": 2 } } ";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(raw_json, &parse_end, 1);

    // step 2: Configure
    cJSON *values = cJSON_GetObjectItem(root, "values");
    cJSON *meta = cJSON_GetObjectItem(root, "meta");
    cJSON *scale_item = cJSON_GetObjectItem(meta, "scale");
    cJSON_bool values_is_array = cJSON_IsArray(values);
    cJSON *val0 = cJSON_GetArrayItem(values, 0);
    cJSON *val1 = cJSON_GetArrayItem(values, 1);
    cJSON *val2 = cJSON_GetArrayItem(values, 2);
    double n0 = cJSON_GetNumberValue(val0);
    double n1 = cJSON_GetNumberValue(val1);
    double n2 = cJSON_GetNumberValue(val2);
    double scale = cJSON_GetNumberValue(scale_item);

    // step 3: Operate & Validate
    double sum = n0 + n1 + n2;
    double scaled_sum = sum * scale * (values_is_array ? 1.0 : 0.0);
    cJSON_AddNumberToObject(root, "sum", sum);
    cJSON_AddNumberToObject(root, "scaled_sum", scaled_sum);

    // step 4: Cleanup
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}