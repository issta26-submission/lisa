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
//<ID> 1063
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObjectCS(root, "name", name);
    const char *version_str = cJSON_Version();
    cJSON *version_item = cJSON_CreateString(version_str);
    cJSON_AddItemToObjectCS(meta, "version", version_item);

    // step 2: Configure
    char *name_value = cJSON_GetStringValue(name);
    cJSON *info = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(info, "extracted_name", cJSON_CreateString(name_value));
    cJSON_AddItemToObjectCS(root, "info", info);

    // step 3: Operate
    char *pretty = cJSON_Print(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);

    // step 4: Validate & Cleanup
    cJSON_free(pretty);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}