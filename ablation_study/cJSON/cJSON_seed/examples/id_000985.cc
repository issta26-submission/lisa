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
//<ID> 985
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("device-X");
    cJSON *id = cJSON_CreateNumber(42.0);

    // step 2: Configure
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "name", name);
    cJSON_AddItemToObject(meta, "id", id);
    cJSON_AddFalseToObject(root, "enabled");

    // step 3: Operate & Validate
    cJSON *got_enabled = cJSON_GetObjectItem(root, "enabled");
    int was_disabled = cJSON_IsFalse(got_enabled);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_ReplaceItemInObject(root, "enabled", true_item);
    cJSON *was_disabled_num = cJSON_CreateNumber((double)was_disabled);
    cJSON_AddItemToObject(root, "was_disabled", was_disabled_num);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}