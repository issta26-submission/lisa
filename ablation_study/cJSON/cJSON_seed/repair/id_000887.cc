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
//<ID> 887
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
    cJSON_AddStringToObject(root, "app", "cjson_test_app");

    // step 2: Configure
    cJSON *childA = cJSON_CreateObject();
    cJSON_AddStringToObject(childA, "name", "alpha");
    cJSON_AddNumberToObject(childA, "count", 5.0);
    cJSON_AddItemToObject(settings, "childA", childA);
    cJSON *childB = cJSON_CreateObject();
    cJSON_AddStringToObject(childB, "name", "beta");
    cJSON_AddNumberToObject(childB, "count", 7.0);
    cJSON_AddItemReferenceToObject(root, "childB_ref", childB);

    // step 3: Operate and Validate
    cJSON *name_item = cJSON_GetObjectItem(childA, "name");
    char *name_val = cJSON_GetStringValue(name_item);
    cJSON *count_item = cJSON_GetObjectItem(childA, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    cJSON_AddStringToObject(root, "childA_name_copy", name_val);
    cJSON_AddNumberToObject(root, "childA_count_copy", count_val);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}