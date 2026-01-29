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
//<ID> 1473
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *id = cJSON_CreateNumber(42.0);
    cJSON *name = cJSON_CreateString("test");
    cJSON_AddItemToObject(child, "id", id);
    cJSON_AddItemToObject(child, "name", name);
    cJSON_AddItemToObject(root, "data", child);
    cJSON *mirror = cJSON_Duplicate(root, 1);

    // step 2: Configure
    cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(root, "data");
    cJSON_bool detached_is_obj = cJSON_IsObject(detached);
    cJSON_AddItemToObject(mirror, "transferred", detached);
    cJSON *temp = cJSON_CreateString("to_be_deleted");
    cJSON_AddItemToObject(root, "temp", temp);

    // step 3: Operate & Validate
    cJSON_DeleteItemFromObjectCaseSensitive(root, "temp");
    cJSON_bool equal = cJSON_Compare(root, mirror, 1);
    char *out = cJSON_PrintUnformatted(mirror);
    (void)detached_is_obj;
    (void)equal;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(mirror);

    // API sequence test completed successfully
    return 66;
}