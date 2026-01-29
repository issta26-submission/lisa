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
//<ID> 1001
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *meta = NULL;
    cJSON *null_item = NULL;
    cJSON *true_item = NULL;
    cJSON *dup = NULL;
    cJSON *got_name = NULL;
    cJSON *newname = NULL;
    cJSON *dup_active = NULL;
    char *printed = NULL;
    char *dup_printed = NULL;
    char *name_val = NULL;
    cJSON_bool dup_active_is_true = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "name", cJSON_CreateString("device42"));
    meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(meta, "optional", null_item);
    true_item = cJSON_AddTrueToObject(root, "active");

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    dup = cJSON_Duplicate(root, 1);
    got_name = cJSON_GetObjectItemCaseSensitive(root, "name");
    name_val = cJSON_GetStringValue(got_name);
    newname = cJSON_CreateString(name_val);
    cJSON_AddItemToObject(dup, "copied_name", newname);
    dup_active = cJSON_GetObjectItemCaseSensitive(dup, "active");
    dup_active_is_true = cJSON_IsTrue(dup_active);
    dup_printed = cJSON_Print(dup);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(dup_printed);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}