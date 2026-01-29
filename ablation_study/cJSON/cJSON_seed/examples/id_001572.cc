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
//<ID> 1572
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
    cJSON_AddItemToObject(root, "numbers", array);
    cJSON_AddNumberToObject(root, "version", 2.5);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag);

    // step 2: Configure
    cJSON *dup_array = cJSON_Duplicate(array, 1);
    cJSON_AddItemToObject(root, "numbers_copy", dup_array);
    cJSON *shallow_dup = cJSON_Duplicate(root, 0);

    // step 3: Operate & Validate
    cJSON *first_item = cJSON_GetArrayItem(array, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON *ver_item = cJSON_GetObjectItem(root, "version");
    double ver_val = cJSON_GetNumberValue(ver_item);
    cJSON *computed = cJSON_CreateNumber(first_val + ver_val);
    cJSON_AddItemToObject(root, "computed_sum", computed);
    cJSON_DeleteItemFromObject(root, "numbers_copy");

    // step 4: Cleanup
    char *printed = cJSON_PrintUnformatted(shallow_dup);
    cJSON_free(printed);
    cJSON_Delete(shallow_dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}