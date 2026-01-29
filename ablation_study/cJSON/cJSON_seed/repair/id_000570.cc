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
//<ID> 570
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *info = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "info", info);
    cJSON *name = cJSON_CreateString("original");
    cJSON_AddItemToObject(info, "name", name);
    cJSON *enabled = cJSON_CreateFalse();
    cJSON_AddItemToObject(info, "enabled", enabled);
    cJSON *value = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(info, "value", value);

    // step 2: Configure
    char *new_name_ptr = cJSON_SetValuestring(name, "updated");
    cJSON *info_copy = cJSON_Duplicate(info, 1);
    cJSON_AddItemToObject(root, "info_copy", info_copy);

    // step 3: Operate and Validate
    cJSON *copied_name_item = cJSON_GetObjectItem(info_copy, "name");
    char *copied_name = cJSON_GetStringValue(copied_name_item);
    (void)copied_name;
    cJSON *orig_enabled_item = cJSON_GetObjectItem(info, "enabled");
    cJSON_bool was_false = cJSON_IsFalse(orig_enabled_item);
    (void)was_false;
    cJSON *orig_value_item = cJSON_GetObjectItem(info, "value");
    double orig_val = cJSON_GetNumberValue(orig_value_item);
    cJSON *computed = cJSON_CreateNumber(orig_val * 2.0);
    cJSON_AddItemToObject(root, "computed", computed);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}