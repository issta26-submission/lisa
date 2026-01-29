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
//<ID> 1650
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    int initial_nums[4] = {10, 20, 30, 40};
    cJSON *root = cJSON_CreateObject();
    cJSON *number_array = cJSON_CreateIntArray(initial_nums, 4);
    cJSON *empty_array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", number_array);
    cJSON_AddItemToObject(root, "extras", empty_array);

    // step 2: Configure
    int extra_vals[3] = {1, 2, 3};
    cJSON *extra_array = cJSON_CreateIntArray(extra_vals, 3);
    cJSON_ReplaceItemInObject(root, "extras", extra_array);

    // step 3: Operate and Validate
    cJSON_bool has_numbers = cJSON_HasObjectItem(root, "numbers");
    cJSON *third_item = cJSON_GetArrayItem(number_array, 2);
    double third_value = cJSON_GetNumberValue(third_item);
    int size = cJSON_GetArraySize(number_array);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = (char)('0' + (int)has_numbers);
    buffer[1] = (char)('0' + (int)third_value % 10);
    buffer[2] = (char)('0' + (size % 10));
    char *printed = cJSON_PrintUnformatted(root);
    buffer[3] = printed[0];
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}