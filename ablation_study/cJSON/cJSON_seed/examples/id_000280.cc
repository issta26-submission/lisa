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
//<ID> 280
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const double nums_array[3] = {3.14, 2.71, 1.0};
    cJSON *nums = cJSON_CreateDoubleArray(nums_array, 3);
    cJSON_AddItemToObject(root, "values", nums);
    cJSON *label = cJSON_CreateString("status");
    cJSON_AddItemToObject(root, "label_ref", label);

    // step 2: Configure
    const char *label_val = cJSON_GetStringValue(label);
    cJSON *status = cJSON_CreateString("ok");
    cJSON_AddItemToObject(root, label_val, status);
    char *snapshot1 = cJSON_Print(root);

    // step 3: Operate & Validate
    cJSON_bool has_values = cJSON_HasObjectItem(root, "values");
    cJSON *has_item = cJSON_CreateBool(has_values);
    cJSON_AddItemToObject(root, "has_values", has_item);
    char *snapshot2 = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(snapshot1);
    cJSON_free(snapshot2);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}