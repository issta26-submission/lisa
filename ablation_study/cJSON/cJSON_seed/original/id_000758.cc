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
//<ID> 758
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("initial");
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON *raw_item = cJSON_CreateRaw("{\"rawkey\":true}");
    cJSON_AddItemToObject(root, "raw", raw_item);
    cJSON *temp_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "temp", temp_item);

    // step 2: Configure
    char *prev_valuestring = cJSON_SetValuestring(name_item, "updated");
    (void)prev_valuestring;

    // step 3: Operate
    cJSON *detached_raw = cJSON_DetachItemFromObject(root, "raw");
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(child, "moved", detached_raw);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "temp");
    char *printed = cJSON_PrintBuffered(root, 256, 1);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_Delete(child);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}