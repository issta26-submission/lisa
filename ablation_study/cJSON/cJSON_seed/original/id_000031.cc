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
//<ID> 31
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"arr\":[1,2,3], \"flag\": false, \"nested\": {\"key\":\"val\"}}";
    cJSON *root = cJSON_Parse(json);

    // step 2: Configure
    cJSON *new_true = cJSON_CreateTrue();
    cJSON_bool replace_result = cJSON_ReplaceItemInObjectCaseSensitive(root, "flag", new_true);
    const char *version_str = cJSON_Version();
    cJSON *version_item = cJSON_CreateString(version_str);
    cJSON_AddItemToObject(root, "version", version_item);

    // step 3: Operate and Validate
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON_DeleteItemFromArray(arr, 1);
    cJSON *flag_item = cJSON_GetObjectItem(root, "flag");
    cJSON_bool flag_is_true = cJSON_IsTrue(flag_item);
    cJSON_AddBoolToObject(root, "flag_checked", flag_is_true);

    // step 4: Cleanup
    (void)replace_result;
    (void)version_str;
    (void)flag_is_true;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}