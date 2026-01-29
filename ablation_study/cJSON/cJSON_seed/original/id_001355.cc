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
//<ID> 1355
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
    nums[2] = 3.75f;

    // step 2: Configure
    cJSON *child = cJSON_AddObjectToObject(root, "config");
    cJSON *floats_arr = cJSON_CreateFloatArray(nums, 3);
    cJSON_AddItemToObject(child, "floats", floats_arr);
    cJSON_AddTrueToObject(child, "enabled");
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(child, "marker", null_item);

    // step 3: Operate
    int arr_size = cJSON_GetArraySize(floats_arr);
    cJSON *first_item = cJSON_GetArrayItem(floats_arr, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON *first_num = cJSON_CreateNumber(first_val);
    cJSON_AddItemToObject(root, "first", first_num);
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