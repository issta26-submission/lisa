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
//<ID> 1472
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
    cJSON *name = cJSON_CreateString("example");
    cJSON *id = cJSON_CreateNumber(123);
    cJSON *meta_version = cJSON_CreateString("v1.0");

    // step 2: Configure
    cJSON_AddItemToObject(root, "name", name);
    cJSON_AddItemToObject(root, "id", id);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "version", meta_version);
    cJSON *other = cJSON_Duplicate(root, 1);

    // step 3: Operate & Validate
    cJSON_bool is_obj = cJSON_IsObject(root);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON *detached_meta = cJSON_DetachItemFromObjectCaseSensitive(root, "meta");
    cJSON *other_meta = cJSON_GetObjectItemCaseSensitive(other, "meta");
    cJSON_bool equal_meta = cJSON_Compare(detached_meta, other_meta, 1);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "name");
    (void)is_obj;
    (void)equal_meta;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(detached_meta);
    cJSON_Delete(other);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}