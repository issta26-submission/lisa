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
//<ID> 285
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const double nums[3] = {1.1, 2.2, 3.3};
    cJSON *arr = cJSON_CreateDoubleArray(nums, 3);
    cJSON *name_item = cJSON_CreateString("dataset");

    // step 2: Configure
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToObject(root, "name", name_item);

    // step 3: Operate & Validate
    cJSON_bool has_values = cJSON_HasObjectItem(root, "values");
    cJSON_AddItemToObject(root, "has_values", cJSON_CreateBool(has_values));
    char *printed = cJSON_Print(root);
    cJSON *got_name_item = cJSON_GetObjectItem(root, "name");
    const char *name_str = cJSON_GetStringValue(got_name_item);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    (void)name_str;
    return 66;
}