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
//<ID> 1051
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *orig_child = NULL;
    cJSON *child_ref = NULL;
    cJSON *replacement = NULL;
    cJSON *flag = NULL;
    cJSON *got_child = NULL;
    cJSON *detached = NULL;
    char *printed = NULL;
    const char *version = NULL;
    cJSON_bool added = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    orig_child = cJSON_CreateObject();
    cJSON_AddStringToObject(orig_child, "inner", "original");
    cJSON_AddItemToObject(root, "child", orig_child);
    flag = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "enabled", flag);
    child_ref = cJSON_CreateObjectReference(orig_child);
    cJSON_AddItemToObject(root, "child_ref", child_ref);
    version = cJSON_Version();
    cJSON_AddStringToObject(root, "cjson_version", version);

    // step 3: Operate / Validate
    replacement = cJSON_CreateObject();
    cJSON_AddStringToObject(replacement, "inner", "replacement");
    got_child = cJSON_GetObjectItem(root, "child");
    detached = cJSON_DetachItemViaPointer(root, got_child);
    (void)detached;
    added = cJSON_AddItemToObject(root, "child", replacement);
    (void)added;
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}