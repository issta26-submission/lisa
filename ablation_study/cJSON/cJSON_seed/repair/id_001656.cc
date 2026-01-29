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
//<ID> 1656
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    int nums[4] = {1, 2, 3, 4};
    cJSON *root = cJSON_CreateObject();
    cJSON *ints = cJSON_CreateIntArray(nums, 4);
    cJSON_AddItemToObject(root, "ints", ints);
    cJSON *more = cJSON_CreateArray();
    cJSON_AddItemToArray(more, cJSON_CreateNumber(10));
    cJSON_AddItemToArray(more, cJSON_CreateNumber(20));
    cJSON_AddItemToArray(more, cJSON_CreateNumber(30));
    cJSON_AddItemToObject(root, "more", more);

    // step 2: Configure
    cJSON_bool has_ints = cJSON_HasObjectItem(root, "ints");
    cJSON_bool has_more = cJSON_HasObjectItem(root, "more");
    cJSON *item_a = cJSON_GetArrayItem(ints, 2);
    cJSON *item_b = cJSON_GetArrayItem(more, 1);
    double val_a = cJSON_GetNumberValue(item_a);
    double val_b = cJSON_GetNumberValue(item_b);

    // step 3: Operate and Validate
    double total = val_a + val_b;
    char *printed = cJSON_PrintUnformatted(root);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = (char)('0' + (int)has_ints);
    buffer[1] = (char)('0' + (int)has_more);
    buffer[2] = (char)('0' + ((int)val_a % 10));
    buffer[3] = (char)('0' + ((int)val_b % 10));
    buffer[4] = printed[0];
    (void)total;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}