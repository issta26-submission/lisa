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
//<ID> 486
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *settings = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *got_list = (cJSON *)0;
    cJSON *got_settings = (cJSON *)0;
    char *printed = (char *)0;
    const char *name_val = (const char *)0;
    cJSON_bool is_list_array = 0;
    cJSON_bool bool_active = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    settings = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("entry1");
    true_item = cJSON_CreateTrue();

    // step 3: Configure
    cJSON_AddStringToObject(settings, "name", "sample");
    cJSON_AddBoolToObject(settings, "active", 1);
    false_item = cJSON_AddFalseToObject(settings, "always_false");
    cJSON_AddItemToObject(settings, "flag_true", true_item);
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON_AddItemToObject(root, "list", arr);

    // step 4: Operate
    got_list = cJSON_GetObjectItem(root, "list");
    is_list_array = cJSON_IsArray(got_list);
    got_settings = cJSON_GetObjectItem(root, "settings");
    name_val = cJSON_GetStringValue(cJSON_GetObjectItem(got_settings, "name"));
    bool_active = cJSON_IsTrue(cJSON_GetObjectItem(got_settings, "active"));

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    (void)name_val;
    (void)is_list_array;
    (void)bool_active;
    (void)false_item;
    (void)got_list;
    (void)got_settings;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}