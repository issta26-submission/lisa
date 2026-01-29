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
//<ID> 1542
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON *active_item = cJSON_AddBoolToObject(root, "active", 1);

    // step 2: Configure
    int nums[3] = {1, 2, 3};
    cJSON *values = cJSON_CreateIntArray(nums, 3);
    cJSON_AddItemToObject(root, "values", values);
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddStringToObject(meta, "version", "1.2.3");

    // step 3: Operate & Validate
    const char *name_value = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));
    cJSON *copied_name = cJSON_CreateString(name_value);
    cJSON_AddItemToObject(root, "copied_name", copied_name);
    cJSON_bool has_active = cJSON_HasObjectItem(root, "active");
    (void)has_active;
    char *printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}