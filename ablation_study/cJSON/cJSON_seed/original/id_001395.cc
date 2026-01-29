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
//<ID> 1395
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    float fvalues[3] = {1.5f, 2.5f, 3.5f};
    cJSON *float_array = cJSON_CreateFloatArray(fvalues, 3);
    cJSON_AddItemToObject(root, "floats", float_array);
    double dvalues[2] = {10.0, 20.0};
    cJSON *double_array = cJSON_CreateDoubleArray(dvalues, 2);
    cJSON_AddItemToObject(root, "doubles", double_array);
    cJSON_AddStringToObject(root, "replace_me", "initial_value");

    // step 2: Configure
    cJSON *replacement = cJSON_CreateNumber(42.0);
    cJSON_bool replace_result = cJSON_ReplaceItemInObject(root, "replace_me", replacement);

    // step 3: Operate
    cJSON *found_floats = cJSON_GetObjectItemCaseSensitive(root, "floats");
    cJSON *found_doubles = cJSON_GetObjectItemCaseSensitive(root, "doubles");
    cJSON *found_replaced = cJSON_GetObjectItemCaseSensitive(root, "replace_me");
    double replaced_number = cJSON_GetNumberValue(found_replaced);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}