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
//<ID> 992
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("device-X");
    int nums[4] = {10, 20, 30, 40};
    cJSON *int_array = cJSON_CreateIntArray(nums, 4);

    // step 2: Configure
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "name", name);
    cJSON_AddItemToObjectCS(root, "ints", int_array);
    cJSON_AddNumberToObject(root, "count", 4.0);

    // step 3: Operate & Validate
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *got_name = cJSON_GetObjectItem(got_meta, "name");
    char *name_str = (char *)cJSON_GetStringValue(got_name);
    cJSON *got_ints = cJSON_GetObjectItem(root, "ints");
    int size = cJSON_GetArraySize(got_ints);
    cJSON_bool has_meta = cJSON_HasObjectItem(root, "meta");
    (void)has_meta; // used to ensure meaningful flow without branching
    cJSON *summary = cJSON_CreateObject();
    cJSON_AddNumberToObject(summary, "ints_size", (double)size);
    cJSON_AddStringToObject(summary, "device_name", name_str);
    cJSON_AddItemToObject(root, "summary", summary);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}