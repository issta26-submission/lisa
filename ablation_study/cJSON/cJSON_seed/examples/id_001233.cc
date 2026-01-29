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
//<ID> 1233
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"source\":[1.5,2.5,3.5],\"maybe_null\":null,\"scale\":2.0}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json_text, &parse_end, 1);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *src = cJSON_GetObjectItem(parsed, "source");
    cJSON *it0 = cJSON_GetArrayItem(src, 0);
    cJSON *it1 = cJSON_GetArrayItem(src, 1);
    cJSON *it2 = cJSON_GetArrayItem(src, 2);
    double v0 = cJSON_GetNumberValue(it0);
    double v1 = cJSON_GetNumberValue(it1);
    double v2 = cJSON_GetNumberValue(it2);
    cJSON *scale_item = cJSON_GetObjectItem(parsed, "scale");
    double scale = cJSON_GetNumberValue(scale_item);
    double scaled_vals[3];
    scaled_vals[0] = v0 * scale;
    scaled_vals[1] = v1 * scale;
    scaled_vals[2] = v2 * scale;
    cJSON *scaled_array = cJSON_CreateDoubleArray(scaled_vals, 3);

    // step 3: Operate & Validate
    cJSON_AddItemToObject(root, "scaled", scaled_array);
    cJSON *empty = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "empty", empty);
    cJSON *maybe_null = cJSON_GetObjectItem(parsed, "maybe_null");
    cJSON_bool is_null = cJSON_IsNull(maybe_null);
    cJSON *null_flag = cJSON_CreateBool(is_null);
    cJSON_AddItemToObject(root, "maybe_null_is_null", null_flag);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}