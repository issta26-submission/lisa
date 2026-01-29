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
//<ID> 936
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"name\":\"source\",\"values\":[1.0,2.5,3.25]}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);

    // step 2: Configure
    cJSON *values = cJSON_GetObjectItem(root, "values");
    float extra_nums[3] = {4.5f, 5.5f, 6.75f};
    cJSON *float_arr = cJSON_CreateFloatArray(extra_nums, 3);
    cJSON *tag = cJSON_CreateString("augmented");
    cJSON *values_ref = cJSON_CreateArrayReference(values);
    cJSON_AddItemToObjectCS(root, "extra_values", float_arr);
    cJSON_AddItemToObjectCS(root, "tag", tag);
    cJSON_AddItemToObjectCS(root, "values_ref", values_ref);

    // step 3: Operate
    int count = cJSON_GetArraySize(values);
    cJSON *second = cJSON_GetArrayItem(values, 1);
    double second_val = cJSON_GetNumberValue(second);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    void *buf = cJSON_malloc(256);
    memset(buf, 0, 256);
    char *prebuf = (char *)buf;
    cJSON_bool ok = cJSON_PrintPreallocated(root, prebuf, 256, 1);
    (void)ok;
    cJSON_free(out);
    cJSON_free(prebuf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}