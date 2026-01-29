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
//<ID> 986
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
    cJSON *name = cJSON_CreateString("device-A");
    cJSON *version = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "name", name);
    cJSON_AddItemToObject(meta, "version", version);
    cJSON_AddFalseToObject(root, "active");

    // step 2: Configure
    cJSON *owner = cJSON_CreateString("owner-xyz");
    cJSON_AddItemToObject(root, "owner", owner);
    cJSON *labels = cJSON_CreateArray();
    cJSON *lbl1 = cJSON_CreateString("alpha");
    cJSON *lbl2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(labels, lbl1);
    cJSON_AddItemToArray(labels, lbl2);
    cJSON_AddItemToObject(meta, "labels", labels);

    // step 3: Operate & Validate
    cJSON *active_flag = cJSON_GetObjectItem(root, "active");
    cJSON_bool was_false = cJSON_IsFalse(active_flag);
    (void)was_false;
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_bool replaced = cJSON_ReplaceItemInObject(root, "active", true_item);
    (void)replaced;
    cJSON *got_name = cJSON_GetObjectItem(meta, "name");
    cJSON_SetValuestring(got_name, "device-B");
    char *current_name = cJSON_GetStringValue(got_name);
    cJSON *alias = cJSON_CreateString(current_name);
    cJSON_AddItemToObject(meta, "alias", alias);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}