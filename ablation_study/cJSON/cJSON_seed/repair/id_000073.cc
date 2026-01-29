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
//<ID> 73
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON *name = cJSON_CreateString("original");
    cJSON_AddItemToObject(settings, "name", name);
    cJSON *count = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(settings, "count", count);

    // step 2: Configure
    char *set_name_ptr = cJSON_SetValuestring(name, "modified");
    cJSON_AddNumberToObject(settings, "first_char_code", (double)(unsigned char)set_name_ptr[0]);
    cJSON_bool has_settings = cJSON_HasObjectItem(root, "settings");
    cJSON_AddBoolToObject(root, "settings_present", has_settings);

    // step 3: Operate and Validate
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    cJSON_bool dup_has_settings = cJSON_HasObjectItem(dup_root, "settings");
    cJSON_AddNumberToObject(root, "dup_has_settings", (double)dup_has_settings);
    cJSON *dup_settings = cJSON_GetObjectItem(dup_root, "settings");
    cJSON *dup_name = cJSON_GetObjectItem(dup_settings, "name");
    char *dup_set_ptr = cJSON_SetValuestring(dup_name, "duplicated");
    cJSON_AddStringToObject(dup_settings, "dup_flag", dup_set_ptr);
    char *out = cJSON_PrintUnformatted(dup_root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(dup_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}