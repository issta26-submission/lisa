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
//<ID> 934
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"device\":\"sensor\",\"meta\":{},\"values\":[]}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);

    // step 2: Configure
    float nums[3] = {1.5f, 2.5f, 3.75f};
    cJSON *float_arr = cJSON_CreateFloatArray(nums, 3);
    cJSON *arr_ref = cJSON_CreateArrayReference(float_arr);
    cJSON *note = cJSON_CreateString("generated-array");
    cJSON_AddItemToObjectCS(root, "values_ref", arr_ref);
    cJSON_AddItemToObjectCS(root, "note", note);

    // step 3: Operate
    char *unformatted = cJSON_PrintUnformatted(root);
    void *buf = cJSON_malloc(512);
    memset(buf, 0, 512);
    char *prebuf = (char *)buf;
    cJSON_bool pre_ok = cJSON_PrintPreallocated(root, prebuf, 512, 1);
    (void)pre_ok;
    cJSON_free(unformatted);
    cJSON_free(prebuf);

    // step 4: Validate & Cleanup
    cJSON_Delete(root);
    cJSON_Delete(float_arr);

    // API sequence test completed successfully
    return 66;
}