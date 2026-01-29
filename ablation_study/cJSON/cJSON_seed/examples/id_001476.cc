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
//<ID> 1476
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *id = cJSON_CreateNumber(42.0);
    cJSON *name = cJSON_CreateString("test");
    cJSON *meta = cJSON_CreateObject();
    cJSON *active = cJSON_CreateTrue();
    cJSON_AddItemToObject(meta, "active", active);
    cJSON_AddItemToObject(root, "id", id);
    cJSON_AddItemToObject(root, "name", name);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *root_copy = cJSON_Duplicate(root, 1);

    // step 2: Configure
    cJSON *extra = cJSON_CreateString("temporary");
    cJSON *temp = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(root_copy, "extra", extra);
    cJSON_AddItemToObject(root_copy, "temp", temp);
    cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(root_copy, "temp");
    cJSON_AddItemToObject(root, "moved_temp", detached);
    cJSON_DeleteItemFromObjectCaseSensitive(root_copy, "extra");

    // step 3: Operate & Validate
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    cJSON_bool copy_is_obj = cJSON_IsObject(root_copy);
    cJSON_bool equal = cJSON_Compare(root, root_copy, 1);
    char *out_root = cJSON_PrintUnformatted(root);
    char *out_copy = cJSON_PrintUnformatted(root_copy);
    (void)root_is_obj;
    (void)copy_is_obj;
    (void)equal;
    cJSON_free(out_root);
    cJSON_free(out_copy);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(root_copy);

    // API sequence test completed successfully
    return 66;
}