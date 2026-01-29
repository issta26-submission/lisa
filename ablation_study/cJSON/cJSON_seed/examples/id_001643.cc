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
//<ID> 1643
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *inner = cJSON_AddObjectToObject(config, "inner");

    // step 2: Configure
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON_AddItemToObject(inner, "s1", s1);
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToObject(inner, "s2", s2);
    cJSON *added = cJSON_AddStringToObject(root, "root_name", "rootvalue");

    // step 3: Operate & Validate
    cJSON *retrieved = cJSON_GetObjectItem(inner, "s1");
    const char *val = cJSON_GetStringValue(retrieved);
    cJSON *copy = cJSON_CreateString(val);
    cJSON_AddItemToObject(root, "copied", copy);
    cJSON *config_copy = cJSON_Duplicate(config, 1);
    cJSON_AddItemToObject(root, "config_copy", config_copy);

    // step 4: Cleanup
    char *unfmt = cJSON_PrintUnformatted(root);
    cJSON_free(unfmt);
    char *full = cJSON_Print(root);
    cJSON_free(full);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}