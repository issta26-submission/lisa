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
//<ID> 756
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("initial_name");
    cJSON *metadata = cJSON_CreateRaw("{\"key\":\"value\"}");
    cJSON *temp = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "name", name);
    cJSON_AddItemToObject(root, "metadata", metadata);
    cJSON_AddItemToObject(root, "temp", temp);

    // step 2: Configure
    cJSON *detached_metadata = cJSON_DetachItemFromObject(root, "metadata");
    char *set_result = cJSON_SetValuestring(name, "updated_name");
    (void)set_result;
    cJSON_DeleteItemFromObjectCaseSensitive(root, "temp");

    // step 3: Operate
    cJSON *new_raw = cJSON_CreateRaw("[1,2,3]");
    cJSON_AddItemToObject(root, "new_raw", new_raw);
    cJSON_AddItemToObject(root, "restored_metadata", detached_metadata);
    char *printed = cJSON_PrintBuffered(root, 256, 1);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}