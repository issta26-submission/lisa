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
//<ID> 969
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
    cJSON *name = cJSON_CreateString("device-old");
    cJSON *id = cJSON_CreateNumber(101.0);

    // step 2: Configure
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "name", name);
    cJSON_AddItemToObject(meta, "id", id);
    cJSON *active_flag = cJSON_AddFalseToObject(root, "active");

    // step 3: Operate & Validate
    cJSON_SetValuestring(name, "device-new");
    char *current_name = cJSON_GetStringValue(name);
    cJSON *alias = cJSON_CreateString(current_name);
    cJSON_AddItemToObject(meta, "alias", alias);
    cJSON *name_copy = cJSON_CreateString(current_name);
    cJSON_AddItemToObject(root, "name_copy", name_copy);
    (void)active_flag; // used to show it was captured and is part of the object graph

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}