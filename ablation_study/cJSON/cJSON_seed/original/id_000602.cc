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
//<ID> 602
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *template_obj = cJSON_CreateObject();
    cJSON_AddNumberToObject(template_obj, "threshold", 3.14);
    cJSON_AddStringToObject(template_obj, "mode", "auto");

    // step 2: Configure
    cJSON *settings = cJSON_AddObjectToObject(root, "settings");
    cJSON *dup_template = cJSON_Duplicate(template_obj, 1);
    cJSON_AddItemToObject(root, "settings_copy", dup_template);
    cJSON *mode_item = cJSON_GetObjectItem(template_obj, "mode");
    cJSON *mode_dup = cJSON_Duplicate(mode_item, 0);
    cJSON_AddItemToObject(settings, "imported_mode", mode_dup);

    // step 3: Operate
    cJSON *settings_copy = cJSON_GetObjectItem(root, "settings_copy");
    cJSON *threshold_item = cJSON_GetObjectItem(settings_copy, "threshold");
    double before = cJSON_GetNumberValue(threshold_item);
    cJSON_SetNumberHelper(threshold_item, before + 1.0);

    // step 4: Validate & Cleanup
    cJSON *threshold_after = cJSON_GetObjectItem(settings_copy, "threshold");
    double final_val = cJSON_GetNumberValue(threshold_after);
    (void)final_val;
    (void)before;
    cJSON_Delete(root);
    cJSON_Delete(template_obj);
    // API sequence test completed successfully
    return 66;
}