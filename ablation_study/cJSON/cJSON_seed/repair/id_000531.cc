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
//<ID> 531
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *count = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(root, "count", count);
    const cJSON *got_count = cJSON_GetObjectItem(root, "count");
    double count_val = cJSON_GetNumberValue(got_count);

    // step 2: Configure
    cJSON *settings = cJSON_AddObjectToObject(root, "settings");
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(settings, "enabled", enabled);
    cJSON *disabled = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "disabled", disabled);
    cJSON *settings_dup = cJSON_Duplicate(settings, 1);
    cJSON_AddItemToObject(root, "settings_copy", settings_dup);

    // step 3: Operate
    double count_val2 = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "count"));
    double total = count_val + count_val2;
    cJSON *total_item = cJSON_CreateNumber(total);
    cJSON_AddItemToObject(root, "total", total_item);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Validate and Cleanup
    double validated_total = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "total"));
    (void)validated_total;
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}