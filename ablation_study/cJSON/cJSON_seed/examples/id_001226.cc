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
//<ID> 1226
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"orig\":[7,14,21],\"meta\":{\"mul\":3}}";
    const char *parse_end = nullptr;
    cJSON *parsed = cJSON_ParseWithOpts(json_text, &parse_end, 1);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *orig = cJSON_GetObjectItem(parsed, "orig");
    cJSON *orig0 = cJSON_GetArrayItem(orig, 0);
    cJSON *orig1 = cJSON_GetArrayItem(orig, 1);
    cJSON *orig2 = cJSON_GetArrayItem(orig, 2);
    int nums[3];
    nums[0] = (int)cJSON_GetNumberValue(orig0);
    nums[1] = (int)cJSON_GetNumberValue(orig1);
    nums[2] = (int)cJSON_GetNumberValue(orig2);
    cJSON *int_array = cJSON_CreateIntArray(nums, 3);
    cJSON_AddItemToObject(root, "ints", int_array);

    // step 3: Operate & Validate
    cJSON *meta = cJSON_GetObjectItem(parsed, "meta");
    cJSON *mul_item = cJSON_GetObjectItem(meta, "mul");
    double factor = cJSON_GetNumberValue(mul_item);
    double second_val = cJSON_GetNumberValue(orig1);
    cJSON *computed = cJSON_CreateNumber(second_val * factor);
    cJSON_AddItemToObject(root, "computed", computed);
    char *out = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}