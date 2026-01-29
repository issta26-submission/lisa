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
//<ID> 1034
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("sensor-A");
    cJSON *value_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON_AddItemToObject(root, "value", value_item);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "active", true_item);

    // step 2: Configure
    cJSON *copy = cJSON_Duplicate(root, 1);
    cJSON_bool equal_flag = cJSON_Compare(root, copy, 1);
    cJSON_AddBoolToObject(root, "equal", equal_flag);
    cJSON_AddNumberToObject(root, "is_true_flag", (double)cJSON_IsTrue(true_item));

    // step 3: Operate & Validate
    cJSON *got_name = cJSON_GetObjectItem(root, "name");
    char *name_str = cJSON_GetStringValue(got_name);
    cJSON *name_copy = cJSON_CreateString(name_str);
    cJSON_AddItemToObject(root, "name_copy", name_copy);
    cJSON *got_value = cJSON_GetObjectItem(root, "value");
    double val = cJSON_GetNumberValue(got_value);
    cJSON *value_copy = cJSON_CreateNumber(val);
    cJSON_AddItemToObject(root, "value_copy", value_copy);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(copy);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}