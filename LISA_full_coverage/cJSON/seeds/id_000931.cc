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
//<ID> 931
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"meta\":\"info\",\"base\":[1.0,2.5,3.75]}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);

    // step 2: Configure
    cJSON *base_array = cJSON_GetObjectItem(root, "base");
    float extra_vals[2] = {4.25f, 5.5f};
    cJSON *float_array = cJSON_CreateFloatArray(extra_vals, 2);
    cJSON *label = cJSON_CreateString("extra_values");
    cJSON *base_ref = cJSON_CreateArrayReference(base_array);

    // step 3: Operate
    cJSON_AddItemToObjectCS(root, "float_array", float_array);
    cJSON_AddItemToObjectCS(root, "label", label);
    cJSON_AddItemToObjectCS(root, "base_ref", base_ref);

    // step 4: Validate & Cleanup
    char *out = cJSON_PrintUnformatted(root);
    void *buf = cJSON_malloc(128);
    memset(buf, 0, 128);
    cJSON_PrintPreallocated(root, (char *)buf, 128, 1);
    cJSON_free(out);
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}