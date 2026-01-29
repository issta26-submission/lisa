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
//<ID> 35
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    const char json[] = "{\"arr\": [false, true, {\"key\":\"val\"}], \"flag\": true}";
    cJSON *root = cJSON_Parse(json);

    // step 2: Configure
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON *new_true = cJSON_CreateTrue();
    cJSON_bool replace_result = cJSON_ReplaceItemInObjectCaseSensitive(root, "flag", new_true);

    // step 3: Operate and Validate
    cJSON *first_elem = cJSON_GetArrayItem(arr, 0);
    cJSON_bool first_was_true = cJSON_IsTrue(first_elem);
    cJSON_DeleteItemFromArray(arr, 0);
    cJSON *flag_item = cJSON_GetObjectItem(root, "flag");
    cJSON_bool flag_now_true = cJSON_IsTrue(flag_item);

    // step 4: Cleanup
    (void)version;
    (void)replace_result;
    (void)first_was_true;
    (void)flag_now_true;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}