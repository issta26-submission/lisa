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
//<ID> 1035
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(3.14);
    cJSON *str_item = cJSON_CreateString("sensor-A");
    cJSON *true_item = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToObject(root, "value", num_item);
    cJSON_AddItemToObject(root, "name", str_item);
    cJSON_AddItemToObject(root, "enabled", true_item);
    cJSON *root_copy = cJSON_CreateObject();
    cJSON *num_copy = cJSON_CreateNumber(3.14);
    cJSON *str_copy = cJSON_CreateString("sensor-A");
    cJSON *true_copy = cJSON_CreateTrue();
    cJSON_AddItemToObject(root_copy, "value", num_copy);
    cJSON_AddItemToObject(root_copy, "name", str_copy);
    cJSON_AddItemToObject(root_copy, "enabled", true_copy);

    // step 3: Operate & Validate
    cJSON_bool equal = cJSON_Compare(root, root_copy, 1);
    cJSON *enabled_item = cJSON_GetObjectItem(root, "enabled");
    cJSON_bool enabled_true = cJSON_IsTrue(enabled_item);
    double status_val = (double)equal + (double)enabled_true;
    cJSON *status_item = cJSON_CreateNumber(status_val);
    cJSON_AddItemToObject(root, "status", status_item);
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    const char *name_str = cJSON_GetStringValue(name_item);
    cJSON *name_copy_from_value = cJSON_CreateString(name_str);
    cJSON_AddItemToObject(root, "name_copy", name_copy_from_value);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(root_copy);

    // API sequence test completed successfully
    return 66;
}