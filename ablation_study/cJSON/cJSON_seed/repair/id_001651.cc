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
//<ID> 1651
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    int base_nums[4] = {10, 20, 30, 40};
    int extra_nums[2] = {5, 6};
    cJSON *root = cJSON_CreateObject();
    cJSON *ints = cJSON_CreateIntArray(base_nums, 4);
    cJSON *extras = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "ints", ints);
    cJSON_AddItemToObject(root, "extras", extras);

    // step 2: Configure
    cJSON *n42 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(extras, n42);
    cJSON *more_ints = cJSON_CreateIntArray(extra_nums, 2);
    cJSON_AddItemToArray(extras, more_ints);

    // step 3: Operate and Validate
    cJSON_bool has_ints = cJSON_HasObjectItem(root, "ints");
    cJSON *third_item = cJSON_GetArrayItem(ints, 2);
    double third_val = cJSON_GetNumberValue(third_item);
    char *printed = cJSON_PrintUnformatted(root);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = (char)('0' + (int)has_ints);
    buffer[1] = (char)('0' + ((int)third_val % 10));
    buffer[2] = printed ? printed[0] : 'X';
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}