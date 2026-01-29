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
//<ID> 425
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
    cJSON *int_array = cJSON_CreateIntArray(nums, 3);
    cJSON_AddItemToObject(root, "ints", int_array);
    cJSON_AddTrueToObject(root, "ready");
    cJSON *target = cJSON_CreateString("to_be_replaced");
    cJSON_AddItemToObject(root, "target", target);

    // step 2: Configure
    cJSON *first_item = cJSON_GetArrayItem(int_array, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON *computed = cJSON_CreateNumber(first_val * 2.5);
    cJSON_AddItemToObject(root, "computed", computed);
    cJSON *replacement = cJSON_CreateNumber(123.0);

    // step 3: Operate
    cJSON_ReplaceItemInObject(root, "target", replacement);
    char *snapshot = cJSON_PrintBuffered(root, 256, 1);

    // step 4: Validate and Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}