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
//<ID> 32
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "init", "start");
    cJSON_AddStringToObject(root, "status", "old");
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);
    cJSON_AddItemToArray(values, cJSON_CreateNumber(1));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(2));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(3));
    cJSON_AddStringToObject(root, "cjson_version", version);

    // step 2: Configure
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_bool replace_ok = cJSON_ReplaceItemInObjectCaseSensitive(root, "status", true_item);

    // step 3: Operate and Validate
    cJSON *status_item = cJSON_GetObjectItemCaseSensitive(root, "status");
    cJSON_bool status_is_true = cJSON_IsTrue(status_item);
    cJSON_DeleteItemFromArray(values, 1);
    cJSON *root_dup = cJSON_Duplicate(root, 1);

    // step 4: Cleanup
    (void)replace_ok;
    (void)status_is_true;
    (void)version;
    cJSON_Delete(root_dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}