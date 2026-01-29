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
//<ID> 754
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_init = "{\"embedded\":true}";
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON *raw_item = cJSON_CreateRaw(raw_init);
    cJSON_AddItemToObject(config, "raw", raw_item);
    cJSON_AddItemToObject(root, "config", config);
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "name", name);
    cJSON *count = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObject(root, "count", count);

    // step 2: Configure
    cJSON *detached = cJSON_DetachItemFromObject(config, "raw");
    char *set_res = cJSON_SetValuestring(detached, "{\"embedded\":false}");
    cJSON_AddItemToObject(root, "detached_raw", detached);

    // step 3: Operate
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    cJSON_free(printed);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "name");
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}