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
//<ID> 987
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", data);

    // step 2: Configure
    cJSON *item1 = cJSON_CreateString("one");
    cJSON *item2 = cJSON_CreateString("two");
    cJSON *item3 = cJSON_CreateString("three");
    cJSON_AddItemToArray(data, item1);
    cJSON_AddItemToArray(data, item2);
    cJSON_AddItemToArray(data, item3);
    cJSON *num_item = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(root, "index", num_item);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 3: Operate and Validate
    cJSON *replacement = cJSON_CreateString("two_replaced");
    cJSON_ReplaceItemInArray(data, 1, replacement);
    cJSON *third = cJSON_GetArrayItem(data, 2);
    const char *third_str = cJSON_GetStringValue(third);
    cJSON *copied_third = cJSON_CreateString(third_str);
    cJSON_AddItemToObject(root, "copied_third", copied_third);
    double set_result = cJSON_SetNumberHelper(num_item, 42.0);
    (void)set_result;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}