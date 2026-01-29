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
//<ID> 870
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
    cJSON_AddItemToObjectCS(root, "settings", settings);

    // step 2: Configure
    cJSON *mode = cJSON_CreateString("fast");
    cJSON_AddItemToObject(settings, "mode", mode);
    cJSON *optnull = cJSON_CreateNull();
    cJSON_AddItemToObject(settings, "optional", optnull);
    const char *ver = cJSON_Version();
    cJSON *ver_ref = cJSON_CreateStringReference(ver);
    cJSON_AddItemToObjectCS(root, "lib_version", ver_ref);

    // step 3: Operate
    char buf[256];
    memset(buf, 0, sizeof(buf));
    cJSON_bool pre_ok = cJSON_PrintPreallocated(root, buf, (int)sizeof(buf), 1);
    (void)pre_ok;
    char *unfmt = cJSON_PrintUnformatted(root);
    const char *mode_val = cJSON_GetStringValue(mode);
    cJSON *mode_copy = cJSON_CreateString(mode_val);
    cJSON_AddItemToObjectCS(root, "mode_copy", mode_copy);
    cJSON_free(unfmt);

    // step 4: Validate & Cleanup
    const char *libver = cJSON_GetStringValue(cJSON_GetObjectItem(root, "lib_version"));
    (void)libver;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}