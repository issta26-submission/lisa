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
//<ID> 337
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON *v0 = cJSON_CreateNumber(10.0);
    cJSON *v1 = cJSON_CreateNumber(20.0);
    cJSON *v2 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(values, v0);
    cJSON_AddItemToArray(values, v1);
    cJSON_AddItemToArray(values, v2);
    cJSON_AddItemToObject(root, "values", values);
    cJSON *settings = cJSON_CreateObject();
    cJSON_AddStringToObject(settings, "mode", "auto");
    cJSON_AddNumberToObject(settings, "level", 5.0);
    cJSON_AddItemToObject(root, "settings", settings);

    // step 2: Configure
    cJSON *new_settings = cJSON_CreateObject();
    cJSON_AddStringToObject(new_settings, "mode", "manual");
    cJSON_AddNumberToObject(new_settings, "level", 10.0);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "settings", new_settings);

    // step 3: Operate and Validate
    cJSON *vals = cJSON_GetObjectItem(root, "values");
    cJSON *second = cJSON_GetArrayItem(vals, 1);
    double second_val = cJSON_GetNumberValue(second);
    cJSON *scaled = cJSON_CreateNumber(second_val * 2.0);
    cJSON_AddItemToArray(vals, scaled);
    char *out = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}