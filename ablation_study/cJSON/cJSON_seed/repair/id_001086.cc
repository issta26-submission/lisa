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
//<ID> 1086
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    float nums[3] = {1.0f, 2.5f, -3.125f};
    cJSON *float_array = cJSON_CreateFloatArray(nums, 3);
    cJSON_AddItemToObject(root, "floats", float_array);
    cJSON *flag_false = cJSON_AddFalseToObject(root, "enabled");

    // step 2: Configure
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *n0 = cJSON_CreateNumber(7.0);
    cJSON *n1 = cJSON_CreateNumber(8.0);
    cJSON *n2 = cJSON_CreateNumber(9.0);
    cJSON_AddItemToArray(items, n0);
    cJSON_AddItemToArray(items, n1);
    cJSON_AddItemToArray(items, n2);
    cJSON *meta = cJSON_CreateString("detachable");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate and Validate
    cJSON *got_floats = cJSON_GetObjectItem(root, "floats");
    cJSON_DeleteItemFromArray(got_floats, 1);
    cJSON *detached_meta = cJSON_DetachItemFromObject(root, "meta");
    char *printed = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = (char)('0' + (cJSON_GetArraySize(items) & 0xF));
    char *meta_str = cJSON_GetStringValue(detached_meta);
    ((char *)scratch)[1] = meta_str ? meta_str[0] : '0';

    // step 4: Cleanup
    cJSON_Delete(detached_meta);
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}