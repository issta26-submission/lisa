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
//<ID> 1076
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *child = NULL;
    cJSON *replacement_false = NULL;
    cJSON *got_before = NULL;
    cJSON *got_after = NULL;
    char *printed = NULL;
    cJSON_bool replaced = 0;
    cJSON_bool is_bool = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "status", "ok");
    cJSON_AddItemToObject(root, "child", child);

    // step 3: Operate / Validate
    replacement_false = cJSON_CreateFalse();
    got_before = cJSON_GetObjectItemCaseSensitive(root, "child");
    replaced = cJSON_ReplaceItemInObjectCaseSensitive(root, "child", replacement_false);
    got_after = cJSON_GetObjectItemCaseSensitive(root, "child");
    is_bool = cJSON_IsBool(got_after);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}