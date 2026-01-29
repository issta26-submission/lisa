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
//<ID> 1121
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child_array = cJSON_CreateArray();
    cJSON *num0 = cJSON_CreateNumber(1.0);
    cJSON *num1 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(child_array, num0);
    cJSON_AddItemToArray(child_array, num1);
    cJSON *child_obj = cJSON_CreateObject();
    cJSON *label_ref = cJSON_CreateStringReference("initial_label");
    cJSON_AddItemToObject(child_obj, "label", label_ref);
    const char *version_str = cJSON_Version();
    cJSON *version_ref = cJSON_CreateStringReference(version_str);
    cJSON *arr_ref = cJSON_CreateArrayReference(child_array);

    // step 2: Configure
    cJSON_AddItemToObject(root, "items", arr_ref);
    cJSON_AddItemToObject(root, "child", child_obj);
    cJSON_AddItemToObject(root, "version", version_ref);

    // step 3: Operate & Validate
    cJSON *new_label_ref = cJSON_CreateStringReference("replaced_label");
    cJSON_bool replaced = cJSON_ReplaceItemInObjectCaseSensitive(root, "child", new_label_ref);
    (void)replaced;
    cJSON *got_version = cJSON_GetObjectItem(root, "version");
    const char *got_version_str = cJSON_GetStringValue(got_version);
    cJSON *ver_copy_ref = cJSON_CreateStringReference(got_version_str);
    cJSON_AddItemToObject(root, "version_copy", ver_copy_ref);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}