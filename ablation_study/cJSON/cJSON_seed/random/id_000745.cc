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
//<ID> 745
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
    cJSON *list = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *retrieved_settings = (cJSON *)0;
    cJSON *retrieved_enabled = (cJSON *)0;
    cJSON *added_bool_ptr = (cJSON *)0;
    cJSON_bool added_settings = 0;
    cJSON_bool added_list = 0;
    int array_size = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    settings = cJSON_CreateObject();
    list = cJSON_CreateArray();
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    added_settings = cJSON_AddItemToObject(root, "settings", settings);
    added_list = cJSON_AddItemToObject(root, "list", list);
    cJSON_AddItemToArray(list, false_item);
    added_bool_ptr = cJSON_AddBoolToObject(settings, "enabled", 0);

    // step 4: Operate
    retrieved_settings = cJSON_GetObjectItem(root, "settings");
    retrieved_enabled = cJSON_GetObjectItem(retrieved_settings, "enabled");
    array_size = cJSON_GetArraySize(list);

    // step 5: Validate
    validation_score = (retrieved_settings != (cJSON *)0)
                     + (retrieved_enabled != (cJSON *)0)
                     + (array_size > 0)
                     + (int)added_settings + (int)added_list
                     + (added_bool_ptr != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}