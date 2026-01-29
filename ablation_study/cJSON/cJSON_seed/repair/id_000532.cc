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
//<ID> 532
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *pi = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(root, "pi", pi);
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", enabled);
    cJSON *disabled = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "disabled", disabled);
    cJSON *settings = cJSON_AddObjectToObject(root, "settings");
    cJSON *initial_temp = cJSON_CreateNumber(22.0);
    cJSON_AddItemToObject(settings, "temp", initial_temp);

    // step 2: Configure
    cJSON *count = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(settings, "count", count);
    cJSON *duplicate = cJSON_Duplicate(root, 1);

    // step 3: Operate
    cJSON *dup_settings = cJSON_GetObjectItem(duplicate, "settings");
    cJSON *dup_count = cJSON_GetObjectItem(dup_settings, "count");
    double count_value = cJSON_GetNumberValue(dup_count);
    double adjusted = count_value * 2.5;
    cJSON *adjusted_item = cJSON_CreateNumber(adjusted);
    cJSON_AddItemToObject(dup_settings, "adjusted_count", adjusted_item);
    char *out = cJSON_PrintUnformatted(duplicate);
    cJSON_free(out);

    // step 4: Validate and Cleanup
    cJSON_Delete(duplicate);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}