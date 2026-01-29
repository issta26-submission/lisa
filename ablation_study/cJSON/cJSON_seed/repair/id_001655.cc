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
//<ID> 1655
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    int base_nums[4] = {10, 20, 30, 40};
    cJSON *root = cJSON_CreateObject();
    cJSON *intArray = cJSON_CreateIntArray(base_nums, 4);
    cJSON_AddItemToObject(root, "ints", intArray);
    cJSON *dynArray = cJSON_CreateArray();
    cJSON *f1 = cJSON_CreateNumber(1.5);
    cJSON *f2 = cJSON_CreateNumber(2.75);
    cJSON_AddItemToArray(dynArray, f1);
    cJSON_AddItemToArray(dynArray, f2);
    cJSON_AddItemToObject(root, "floats", dynArray);
    cJSON *label = cJSON_CreateString("dataset");
    cJSON_AddItemToObject(root, "label", label);

    // step 2: Configure
    cJSON *ints_copy = cJSON_Duplicate(intArray, 1);
    cJSON_AddItemToObject(root, "ints_copy", ints_copy);

    // step 3: Operate and Validate
    cJSON_bool has_ints = cJSON_HasObjectItem(root, "ints");
    cJSON *second_item = cJSON_GetArrayItem(intArray, 1);
    double second_val = cJSON_GetNumberValue(second_item);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    char *printed = cJSON_PrintUnformatted(root);
    buffer[0] = has_ints ? 'Y' : 'N';
    buffer[1] = (char)('0' + (int)(second_val / 10));
    buffer[2] = printed ? printed[0] : 'X';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}