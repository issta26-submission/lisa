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
//<ID> 1358
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    float nums[4] = {1.0f, 2.5f, -3.125f, 0.0f};
    cJSON *root = cJSON_CreateObject();
    cJSON *nested = cJSON_AddObjectToObject(root, "nested");
    cJSON *float_array = cJSON_CreateFloatArray(nums, 4);
    cJSON *nothing = cJSON_CreateNull();

    // step 2: Configure
    cJSON_AddItemToObject(nested, "floats", float_array);
    cJSON_AddItemToObject(nested, "nothing", nothing);
    cJSON_AddTrueToObject(root, "ok");

    // step 3: Operate
    cJSON *retrieved_nested = cJSON_GetObjectItem(root, "nested");
    cJSON *retrieved_floats = cJSON_GetObjectItem(retrieved_nested, "floats");
    int count = cJSON_GetArraySize(retrieved_floats);
    cJSON *count_marker = cJSON_CreateNumber((double)count);
    cJSON_AddItemToObject(root, "count", count_marker);
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}