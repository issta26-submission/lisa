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
//<ID> 44
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *parsed = cJSON_Parse("{\"alpha\": 3.1415, \"flag\": true}");
    cJSON_AddItemToObjectCS(root, "parsed", parsed);

    // step 2: Configure
    double nums[3] = {1.1, 2.2, 3.3};
    cJSON *arr = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObjectCS(root, "doubles", arr);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToObjectCS(root, "enabled", false_item);

    // step 3: Operate & Validate
    cJSON *alpha_item = cJSON_GetObjectItem(parsed, "alpha");
    double alpha_val = cJSON_GetNumberValue(alpha_item);
    cJSON *alpha_copy = cJSON_Duplicate(alpha_item, 0);
    cJSON_AddItemToObjectCS(root, "alpha_copy", alpha_copy);
    cJSON_bool has_parsed = cJSON_HasObjectItem(root, "parsed");
    cJSON_bool has_doubles = cJSON_HasObjectItem(root, "doubles");
    cJSON_bool has_enabled = cJSON_HasObjectItem(root, "enabled");
    int arr_size = cJSON_GetArraySize(arr);
    int summary = arr_size + (int)has_parsed + (int)has_enabled;
    (void)alpha_val;
    (void)summary;
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}