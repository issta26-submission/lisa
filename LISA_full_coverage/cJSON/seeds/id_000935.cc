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
//<ID> 935
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"meta\":\"orig\",\"items\":[1.0,2.0]}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);

    // step 2: Configure
    float new_values[3] = {3.14f, 2.71f, 1.0f};
    cJSON *float_arr = cJSON_CreateFloatArray(new_values, 3);
    cJSON_AddItemToObjectCS(root, "extra_floats", float_arr);
    cJSON *label = cJSON_CreateString("generated");
    cJSON_AddItemToObjectCS(root, "label", label);
    cJSON *arr_ref = cJSON_CreateArrayReference(float_arr);
    cJSON_AddItemToObjectCS(root, "extra_floats_ref", arr_ref);

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    void *prebuf = cJSON_malloc(512);
    memset(prebuf, 0, 512);
    cJSON_bool ok = cJSON_PrintPreallocated(root, (char *)prebuf, 512, 0);
    (void)ok;
    cJSON_free(out);
    cJSON_free(prebuf);

    // step 4: Validate & Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}