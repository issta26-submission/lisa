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
//<ID> 1249
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);

    // step 2: Configure
    cJSON_AddBoolToObject(config, "enabled", 1);
    cJSON *sub = cJSON_CreateObject();
    cJSON_AddStringToObject(sub, "mode", "fast");
    cJSON_AddItemToObject(config, "sub", sub);

    // step 3: Operate
    cJSON *sub_ref = cJSON_CreateObjectReference(sub);
    cJSON_AddItemToObject(root, "sub_ref", sub_ref);
    cJSON *got_config = cJSON_GetObjectItem(root, "config");
    cJSON *got_enabled = cJSON_GetObjectItem(got_config, "enabled");
    cJSON *got_subref = cJSON_GetObjectItem(root, "sub_ref");
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 16);
    memset(buf, 0, printed_len + 16);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);

    // step 4: Validate & Cleanup
    cJSON *mode_item = cJSON_GetObjectItem(got_subref, "mode");
    const char *mode_str = cJSON_GetStringValue(mode_item);
    (void)mode_str;
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}