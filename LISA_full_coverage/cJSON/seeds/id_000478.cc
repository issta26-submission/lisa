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
//<ID> 478
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"settings\":{\"threshold\":10},\"list\":[1,2]}";
    cJSON *root = cJSON_ParseWithLength(json, sizeof(json) - 1);
    cJSON *settings = cJSON_GetObjectItem(root, "settings");
    cJSON *threshold = cJSON_GetObjectItem(settings, "threshold");
    cJSON *list = cJSON_GetObjectItem(root, "list");

    // step 2: Configure
    cJSON *extra = cJSON_AddArrayToObject(root, "extra");
    cJSON *enabled = cJSON_AddTrueToObject(settings, "enabled");
    cJSON_SetNumberHelper(threshold, 15.75);
    cJSON *newnum = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "newval", newnum);
    cJSON_bool settings_is_obj = cJSON_IsObject(settings);

    // step 3: Operate
    cJSON_AddItemReferenceToArray(extra, newnum);
    double threshold_after = cJSON_GetNumberValue(threshold);
    cJSON *retrieved_newval = cJSON_GetObjectItem(root, "newval");
    double newval_num = cJSON_GetNumberValue(retrieved_newval);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    (void)settings_is_obj;
    (void)threshold_after;
    (void)newval_num;
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}