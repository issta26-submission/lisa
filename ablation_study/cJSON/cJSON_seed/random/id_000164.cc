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
//<ID> 164
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
    cJSON *threshold = (cJSON *)0;
    cJSON *retrieved_settings = (cJSON *)0;
    cJSON *retrieved_threshold = (cJSON *)0;
    cJSON *settings_flag_num = (cJSON *)0;
    cJSON_bool settings_is_object = (cJSON_bool)0;
    double threshold_value = 0.0;
    char *json_text = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    settings = cJSON_CreateObject();
    threshold = cJSON_CreateNumber(42.5);

    // step 3: Configure
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON_AddItemToObject(settings, "threshold", threshold);

    // step 4: Operate
    retrieved_settings = cJSON_GetObjectItem(root, "settings");
    settings_is_object = cJSON_IsObject(retrieved_settings);
    retrieved_threshold = cJSON_GetObjectItem(retrieved_settings, "threshold");
    threshold_value = cJSON_GetNumberValue(retrieved_threshold);

    // step 5: Validate
    settings_flag_num = cJSON_CreateNumber((double)settings_is_object);
    cJSON_AddItemToObject(root, "settings_is_object", settings_flag_num);
    cJSON_AddNumberToObject(root, "threshold_plus_one", threshold_value + 1.0);
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}