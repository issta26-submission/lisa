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
//<ID> 861
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
    cJSON *float_arr = cJSON_CreateFloatArray(nums, 3);
    cJSON_AddItemToObject(root, "floats", float_arr);
    cJSON *false_item = cJSON_AddFalseToObject(root, "active");
    cJSON_bool is_false = cJSON_IsFalse(false_item);

    // step 2: Configure
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    (void)printed_ok;
    const char *json = " { \"external\": { \"a\": 7, \"b\": 8 } } ";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json, &parse_end, 1);
    cJSON *external = cJSON_GetObjectItem(parsed, "external");
    cJSON *external_ref = cJSON_CreateObjectReference(external);
    cJSON_AddItemToObject(root, "external", external_ref);

    // step 3: Operate
    cJSON *root_floats = cJSON_GetObjectItem(root, "floats");
    int arr_size = cJSON_GetArraySize(root_floats);
    cJSON *first = cJSON_GetArrayItem(root_floats, 0);
    double first_val = cJSON_GetNumberValue(first);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    (void)is_false;
    (void)arr_size;
    (void)first_val;

    // step 4: Validate & Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}