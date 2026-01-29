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
//<ID> 123
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *json_template = "{\"info\":\"parsed_template\",\"version\":1}";
    cJSON *parsed = cJSON_Parse(json_template);
    double nums[3];
    nums[0] = 1.1;
    nums[1] = 2.2;
    nums[2] = 3.3;
    cJSON *double_arr = cJSON_CreateDoubleArray(nums, 3);
    cJSON *placeholder_arr = cJSON_AddArrayToObject(root, "numbers");
    cJSON_AddStringToObject(root, "label", "double_sequence");

    // step 2: Configure
    cJSON_ReplaceItemViaPointer(root, placeholder_arr, double_arr);
    cJSON *parsed_info_item = cJSON_GetObjectItem(parsed, "info");
    const char *parsed_info_str = cJSON_GetStringValue(parsed_info_item);
    cJSON_AddStringToObject(root, "parsed_info", parsed_info_str);

    // step 3: Operate and Validate
    cJSON *arr = cJSON_GetObjectItem(root, "numbers");
    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    char first_buf[64];
    sprintf(first_buf, "%.6g", first_val);
    cJSON_AddStringToObject(root, "first_value_str", first_buf);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}