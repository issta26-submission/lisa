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
//<ID> 283
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double nums[3] = {1.5, 2.5, 3.5};
    int count = 3;
    cJSON *dbl_array = cJSON_CreateDoubleArray(nums, count);
    cJSON *name_item = cJSON_CreateString("sensorA");

    // step 2: Configure
    cJSON_AddItemToObject(root, "values", dbl_array);
    cJSON_AddItemToObject(root, "name", name_item);

    // step 3: Operate & Validate
    cJSON_bool has_values = cJSON_HasObjectItem(root, "values");
    cJSON *has_values_item = cJSON_CreateBool(has_values);
    cJSON_AddItemToObject(root, "has_values", has_values_item);
    char *printed = cJSON_Print(root);
    cJSON *name_ptr = cJSON_GetObjectItem(root, "name");
    const char *name_str = cJSON_GetStringValue(name_ptr);
    cJSON *name_copy = cJSON_CreateString(name_str);
    cJSON_AddItemToObject(root, "name_copy", name_copy);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}