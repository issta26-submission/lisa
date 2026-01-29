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
//<ID> 630
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);
    cJSON *v0 = cJSON_CreateNumber(3.14);
    cJSON *v1 = cJSON_CreateNumber(2.71);
    cJSON *v2 = cJSON_CreateNumber(1.41);
    cJSON_AddItemToArray(values, v0);
    cJSON_AddItemToArray(values, v1);
    cJSON_AddItemToArray(values, v2);
    cJSON *enabled = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "enabled", enabled);

    // step 2: Configure
    cJSON_AddStringToObject(root, "version", cJSON_Version());
    cJSON *second_value = cJSON_GetArrayItem(values, 1);
    double new_val = cJSON_SetNumberHelper(second_value, 42.5);
    (void)new_val;

    // step 3: Operate and Validate
    cJSON *old_enabled = cJSON_GetObjectItem(root, "enabled");
    cJSON *replacement_enabled = cJSON_CreateBool(0);
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(root, old_enabled, replacement_enabled);
    (void)replaced;
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    (void)printed;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}