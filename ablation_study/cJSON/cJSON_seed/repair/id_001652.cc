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
//<ID> 1652
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    int nums[4] = {1, 2, -3, 256};
    cJSON *root = cJSON_CreateObject();
    cJSON *ints = cJSON_CreateIntArray(nums, 4);
    cJSON_AddItemToObject(root, "ints", ints);
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", list);

    // step 2: Configure
    cJSON *extra_num = cJSON_CreateNumber(42.5);
    cJSON_AddItemToArray(list, extra_num);

    // step 3: Operate and Validate
    cJSON_bool has_ints = cJSON_HasObjectItem(root, "ints");
    cJSON *second_item = cJSON_GetArrayItem(ints, 1);
    double second_val = cJSON_GetNumberValue(second_item);
    char *printed = cJSON_PrintUnformatted(root);
    char buffer[16];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = (char)('0' + (int)has_ints);
    buffer[1] = (char)('0' + ((int)second_val % 10));
    buffer[2] = printed ? printed[0] : 'X';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}