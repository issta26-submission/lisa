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
//<ID> 79
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    float nums[3] = {1.25f, 2.5f, 3.75f};
    cJSON *root = cJSON_CreateObject();
    cJSON *float_arr = cJSON_CreateFloatArray(nums, 3);
    cJSON_AddItemToObject(root, "values", float_arr);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "name", "dataset");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *meta_dup = cJSON_Duplicate(meta, 1); // use a duplicate to avoid double-free issues
    cJSON_AddItemToArray(items, meta_dup);
    cJSON *count = cJSON_CreateNumber(3.0);
    cJSON_AddItemToObject(root, "count", count);

    // step 3: Operate and Validate
    cJSON_bool meta_is_obj = cJSON_IsObject(meta);
    char *printed = cJSON_Print(root);
    char *printed_buf = cJSON_PrintBuffered(root, 64, 1);
    int summary = (int)meta_is_obj + (int)printed[0] + (int)printed_buf[0];
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(printed_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}