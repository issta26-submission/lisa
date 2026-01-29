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
//<ID> 1022
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *settings = NULL;
    cJSON *enabled = NULL;
    cJSON *values = NULL;
    cJSON *detached_settings = NULL;
    cJSON *arr = NULL;
    cJSON *dup_root = NULL;
    cJSON *dup_settings = NULL;
    char *printed = NULL;
    cJSON_bool enabled_true = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    settings = cJSON_CreateObject();
    enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(settings, "enabled", enabled);
    values = cJSON_CreateArray();
    cJSON_AddItemToArray(values, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(2.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(3.0));
    cJSON_AddItemToObject(settings, "values", values);
    cJSON_AddItemToObject(root, "settings", settings);

    // step 3: Operate / Validate
    dup_root = cJSON_Duplicate(root, 1);
    detached_settings = cJSON_DetachItemFromObject(root, "settings");
    arr = cJSON_CreateArray();
    cJSON_AddItemToArray(arr, detached_settings);
    cJSON_AddItemToObject(root, "archive", arr);
    dup_settings = cJSON_Duplicate(cJSON_GetObjectItem(dup_root, "settings"), 1);
    cJSON_AddItemToObject(dup_root, "copied_settings", dup_settings);
    enabled_true = cJSON_IsTrue(cJSON_GetObjectItem(dup_settings, "enabled"));
    printed = cJSON_PrintUnformatted(dup_root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}