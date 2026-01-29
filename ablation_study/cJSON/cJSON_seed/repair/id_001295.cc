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
//<ID> 1295
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON *enabled = cJSON_CreateFalse();
    cJSON *label = cJSON_CreateString("alpha");
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON_AddItemToObject(root, "enabled", enabled);
    cJSON_AddItemToObject(root, "label", label);

    // step 2: Configure
    cJSON *mirror = cJSON_CreateObject();
    cJSON *mirror_settings = cJSON_CreateObject();
    cJSON_AddItemToObject(mirror_settings, "label", cJSON_CreateString("alpha"));
    cJSON_AddItemToObject(mirror, "settings", mirror_settings);
    cJSON_AddItemToObject(mirror, "enabled", cJSON_CreateFalse());
    cJSON_AddItemToObject(mirror, "label", cJSON_CreateString("alpha"));

    // step 3: Operate
    cJSON *enabled_item = cJSON_GetObjectItem(root, "enabled");
    cJSON_bool is_bool = cJSON_IsBool(enabled_item);
    cJSON_bool is_false = cJSON_IsFalse(enabled_item);
    cJSON *label_item = cJSON_GetObjectItem(root, "label");
    char *label_val = cJSON_GetStringValue(label_item);
    cJSON_bool equal = cJSON_Compare(root, mirror, 1);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = label_val[0];

    // step 4: Validate & Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    cJSON_Delete(mirror);
    // API sequence test completed successfully
    return 66;
}