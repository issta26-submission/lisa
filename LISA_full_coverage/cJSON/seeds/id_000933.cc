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
//<ID> 933
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"title\":\"demo\",\"existing_values\":[3.14,2.71]}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);

    // step 2: Configure
    float nums[3];
    nums[0] = 0.5f;
    nums[1] = 1.5f;
    nums[2] = 2.5f;
    cJSON *float_arr = cJSON_CreateFloatArray(nums, 3);
    cJSON *arr_ref = cJSON_CreateArrayReference(float_arr);
    cJSON *note = cJSON_CreateString("added_via_api");
    cJSON_AddItemToObjectCS(root, "note", note);

    // step 3: Operate
    char *out_unformatted = cJSON_PrintUnformatted(root);
    int prebuf_len = 256;
    void *prebuf = cJSON_malloc((size_t)prebuf_len);
    memset(prebuf, 0, (size_t)prebuf_len);
    cJSON_bool ok = cJSON_PrintPreallocated(root, (char *)prebuf, prebuf_len, 1);
    (void)ok;

    // step 4: Validate & Cleanup
    cJSON_free(out_unformatted);
    cJSON_free(prebuf);
    cJSON_Delete(arr_ref);
    cJSON_Delete(float_arr);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}