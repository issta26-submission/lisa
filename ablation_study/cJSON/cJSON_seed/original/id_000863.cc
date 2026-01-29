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
//<ID> 863
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    float nums[] = { 1.0f, 2.5f, 3.125f };
    cJSON *float_array = cJSON_CreateFloatArray(nums, 3);
    cJSON_AddItemToObject(root, "float_array", float_array);
    cJSON *flag = cJSON_AddFalseToObject(root, "enabled");

    // step 2: Configure
    const char json_src[] = "{ \"nested\": { \"x\": 4.5 } }";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json_src, &parse_end, 1);
    cJSON *nested = cJSON_GetObjectItem(parsed, "nested");
    cJSON *nested_ref = cJSON_CreateObjectReference(nested);
    cJSON_AddItemToObject(root, "nested_ref", nested_ref);

    // step 3: Operate
    double xval = cJSON_GetNumberValue(cJSON_GetObjectItem(nested, "x"));
    cJSON *doubled = cJSON_CreateNumber(xval * 2.0);
    cJSON_AddItemToObject(root, "x_double", doubled);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Validate & Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}