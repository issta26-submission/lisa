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
//<ID> 286
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double nums[3] = {1.0, 2.5, 3.1415};
    cJSON *doubles = cJSON_CreateDoubleArray(nums, 3);
    cJSON *name = cJSON_CreateString("example_name");
    cJSON *count = cJSON_CreateNumber(3);

    // step 2: Configure
    cJSON_bool add_doubles_ok = cJSON_AddItemToObject(root, "doubles", doubles);
    cJSON_bool add_name_ok = cJSON_AddItemToObject(root, "name", name);
    cJSON_AddItemToObject(root, "count", count);
    cJSON_bool added_both = add_doubles_ok & add_name_ok;
    cJSON *added_flag = cJSON_CreateBool(added_both);
    cJSON_AddItemToObject(root, "added_both", added_flag);

    // step 3: Operate & Validate
    cJSON_bool has_doubles = cJSON_HasObjectItem(root, "doubles");
    cJSON *has_flag = cJSON_CreateBool(has_doubles);
    cJSON_AddItemToObject(root, "has_doubles", has_flag);
    char *printed = cJSON_Print(root);
    const char *name_val = cJSON_GetStringValue(name);
    cJSON *name_copy = cJSON_CreateString(name_val);
    cJSON_AddItemToObject(root, "name_copy", name_copy);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}