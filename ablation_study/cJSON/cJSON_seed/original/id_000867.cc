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
//<ID> 867
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    float nums[3];
    nums[0] = 1.0f;
    nums[1] = 2.5f;
    nums[2] = -3.25f;
    cJSON *float_array = cJSON_CreateFloatArray(nums, 3);
    cJSON_AddItemToObject(root, "floats", float_array);
    const char *json_text = "{ \"external\": { \"a\": 7.5, \"b\": 2 } }";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json_text, &parse_end, 1);

    // step 2: Configure
    cJSON *external = cJSON_GetObjectItem(parsed, "external");
    cJSON *external_ref = cJSON_CreateObjectReference(external);
    cJSON_AddItemToObject(root, "external_ref", external_ref);
    cJSON *false_item = cJSON_AddFalseToObject(root, "enabled");
    char buffer[128];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    (void)printed_ok;
    (void)false_item;

    // step 3: Operate
    double a_val = cJSON_GetNumberValue(cJSON_GetObjectItem(external, "a"));
    cJSON_AddItemReferenceToObject(parsed, "floats_copy", float_array);
    char *out = cJSON_PrintUnformatted(root);
    (void)a_val;

    // step 4: Validate & Cleanup
    cJSON_free(out);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}