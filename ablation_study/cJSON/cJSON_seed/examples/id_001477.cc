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
//<ID> 1477
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *id = cJSON_CreateNumber(123.0);
    cJSON *name = cJSON_CreateString("alpha");
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "token", "tk-xyz");

    // step 2: Configure
    cJSON_AddItemToObject(root, "id", id);
    cJSON_AddItemToObject(root, "name", name);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *dup = cJSON_Duplicate(root, 1);

    // step 3: Operate & Validate
    char *out_root = cJSON_PrintUnformatted(root);
    char *out_dup = cJSON_PrintUnformatted(dup);
    cJSON_bool equal_initial = cJSON_Compare(root, dup, 1);
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    cJSON *detached_meta = cJSON_DetachItemFromObjectCaseSensitive(root, "meta");
    cJSON_DeleteItemFromObjectCaseSensitive(root, "name");
    char *out_root_after = cJSON_PrintUnformatted(root);
    char *out_detached_meta = cJSON_PrintUnformatted(detached_meta);
    cJSON_bool equal_after = cJSON_Compare(root, dup, 1);
    (void)equal_initial;
    (void)root_is_obj;
    (void)equal_after;

    // step 4: Cleanup
    cJSON_free(out_root);
    cJSON_free(out_dup);
    cJSON_free(out_root_after);
    cJSON_free(out_detached_meta);
    cJSON_Delete(root);
    cJSON_Delete(dup);
    cJSON_Delete(detached_meta);

    // API sequence test completed successfully
    return 66;
}