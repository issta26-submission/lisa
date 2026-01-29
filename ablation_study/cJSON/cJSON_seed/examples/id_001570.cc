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
//<ID> 1570
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int nums[4] = {1, 2, 3, 4};
    cJSON *array = cJSON_CreateIntArray(nums, 4);
    cJSON_AddItemToObject(root, "array", array);
    cJSON_AddItemToObject(root, "flag", cJSON_CreateTrue());
    cJSON_AddNumberToObject(root, "base", 10.5);

    // step 2: Configure
    cJSON *dup_array = cJSON_Duplicate(array, 1);
    cJSON_AddItemToObject(root, "array_copy", dup_array);
    cJSON *deep_root = cJSON_Duplicate(root, 1);

    // step 3: Operate & Validate
    cJSON *first_item = cJSON_GetArrayItem(array, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    double base_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "base"));
    cJSON *computed = cJSON_CreateNumber(first_val + base_val);
    cJSON_AddItemToObject(root, "computed", computed);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_DeleteItemFromObject(root, "array_copy");

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(deep_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}