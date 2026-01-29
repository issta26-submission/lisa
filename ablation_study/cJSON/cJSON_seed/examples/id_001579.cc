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
//<ID> 1579
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int nums[3] = {10, 20, 30};
    cJSON *array = cJSON_CreateIntArray(nums, 3);
    cJSON_AddItemToObject(root, "numbers", array);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", flag);
    cJSON_AddNumberToObject(root, "version", 2.5);

    // step 2: Configure
    cJSON *dup_array = cJSON_Duplicate(array, 1);
    cJSON_AddItemToObject(root, "numbers_copy", dup_array);
    cJSON *second = cJSON_GetArrayItem(array, 1);
    double second_val = cJSON_GetNumberValue(second);
    cJSON *computed = cJSON_CreateNumber(second_val + 4.5);
    cJSON_AddItemToObject(root, "computed", computed);

    // step 3: Operate & Validate
    cJSON_DeleteItemFromObject(root, "numbers");
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}