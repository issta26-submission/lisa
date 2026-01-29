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
//<ID> 627
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"config\":{\"threads\":4,\"mode\":\"fast\",\"enabled\":true},\"nums\":[1,2,3]}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_text, &parse_end, 1);

    // step 2: Configure
    cJSON *config_obj = cJSON_GetObjectItem(root, "config");
    cJSON *threads_item = cJSON_GetObjectItem(config_obj, "threads");
    double threads_val = cJSON_GetNumberValue(threads_item);
    cJSON *extra_array = cJSON_CreateArray();
    cJSON *added_number = cJSON_CreateNumber(threads_val + 10.0);
    cJSON_AddItemToArray(extra_array, added_number);
    cJSON_AddItemToObject(root, "extra_nums", extra_array);

    // step 3: Operate & Validate
    cJSON *nums_ref = cJSON_GetObjectItem(root, "nums");
    cJSON_bool was_array = cJSON_IsArray(nums_ref);
    (void)was_array;
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    cJSON_DeleteItemFromObject(root, "config");
    const char *parse_end2 = NULL;
    cJSON *parsed_copy = cJSON_ParseWithOpts(buffer, &parse_end2, 1);
    cJSON *detached_nums = cJSON_DetachItemFromObject(parsed_copy, "nums");
    cJSON_AddItemToObject(root, "restored_nums", detached_nums);
    cJSON *restored_ref = cJSON_GetObjectItem(root, "restored_nums");
    cJSON_bool restored_is_array = cJSON_IsArray(restored_ref);
    (void)restored_is_array;

    // step 4: Cleanup
    cJSON_Delete(parsed_copy);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}