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
//<ID> 877
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
    cJSON_AddItemToObjectCS(root, "config", config);

    // step 2: Configure
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObjectCS(config, "name", name);
    const char *ver_cstr = cJSON_Version();
    cJSON *ver = cJSON_CreateString(ver_cstr);
    cJSON_AddItemToObjectCS(config, "cjson_version", ver);
    cJSON *optional_null = cJSON_CreateNull();
    cJSON_AddItemToObjectCS(config, "optional", optional_null);

    // step 3: Operate
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    (void)printed;
    const char *name_val = cJSON_GetStringValue(name);
    const char *version_val = cJSON_GetStringValue(ver);
    cJSON *name_ref = cJSON_CreateStringReference(name_val);
    cJSON_AddItemToObjectCS(root, "name_ref", name_ref);
    cJSON *ver_ref = cJSON_CreateStringReference(version_val);
    cJSON_AddItemToObjectCS(root, "version_ref", ver_ref);
    char *raw = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(raw);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}