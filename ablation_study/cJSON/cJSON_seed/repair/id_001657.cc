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
//<ID> 1657
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    int base_numbers[3] = {10, 20, 30};
    cJSON *root = cJSON_CreateObject();
    cJSON *nums = cJSON_CreateIntArray(base_numbers, 3);
    cJSON_AddItemToObject(root, "nums", nums);
    cJSON *extra = cJSON_CreateArray();
    cJSON *e0 = cJSON_CreateNumber(7);
    cJSON *e1 = cJSON_CreateNumber(8);
    cJSON_AddItemToArray(extra, e0);
    cJSON_AddItemToArray(extra, e1);
    cJSON_AddItemToObject(root, "extra", extra);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON *mid = cJSON_CreateNumber(42);
    cJSON_AddItemToObject(meta, "id", mid);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate and Validate
    cJSON_bool has_nums = cJSON_HasObjectItem(root, "nums");
    cJSON *second_item = cJSON_GetArrayItem(nums, 1);
    double second_val = cJSON_GetNumberValue(second_item);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = (char)('0' + (int)(has_nums ? 1 : 0));
    buffer[1] = (char)('0' + (int)second_val % 10);
    char *printed = cJSON_PrintUnformatted(root);
    buffer[2] = printed ? printed[0] : 'X';
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}