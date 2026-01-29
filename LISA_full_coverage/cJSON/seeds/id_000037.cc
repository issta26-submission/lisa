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
//<ID> 37
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    const char json[] = "{\"arr\": [ true, false, true ], \"obj\": {\"key\": \"value\", \"replace\": false}}";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, json_len);

    // step 2: Configure and Operate
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON *obj = cJSON_GetObjectItem(root, "obj");
    cJSON *first_elem = cJSON_GetArrayItem(arr, 0);
    cJSON_bool first_is_true = cJSON_IsTrue(first_elem);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_bool replace_ok = cJSON_ReplaceItemInObjectCaseSensitive(obj, "replace", true_item);
    cJSON_DeleteItemFromArray(arr, 1);

    // step 3: Validate
    cJSON *root_dup = cJSON_Duplicate(root, 1);
    cJSON_bool equal_after_dup = cJSON_Compare(root_dup, root, 1);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_Minify(printed);
    cJSON *reparsed = cJSON_Parse(printed);
    cJSON *re_obj = cJSON_GetObjectItem(reparsed, "obj");
    cJSON *re_replace = cJSON_GetObjectItem(re_obj, "replace");
    cJSON_bool reparsed_replace_is_true = cJSON_IsTrue(re_replace);

    // step 4: Cleanup
    (void)version;
    (void)first_is_true;
    (void)replace_ok;
    (void)equal_after_dup;
    (void)reparsed_replace_is_true;
    cJSON_Delete(reparsed);
    cJSON_free(printed);
    cJSON_Delete(root_dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}