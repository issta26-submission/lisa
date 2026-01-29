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
//<ID> 1065
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *info = cJSON_AddObjectToObject(root, "info");
    const char *version_cstr = cJSON_Version();
    cJSON *version_item = cJSON_CreateString(version_cstr);
    cJSON_AddItemToObjectCS(root, "version", version_item);

    // step 2: Configure
    char *version_value = cJSON_GetStringValue(version_item);
    cJSON *version_copy = cJSON_CreateString(version_value);
    cJSON_AddItemToObjectCS(info, "version_copy", version_copy);
    cJSON_AddItemToObjectCS(info, "status", cJSON_CreateString("ok"));

    // step 3: Operate
    char *pretty = cJSON_Print(root);
    char *prealloc = (char *)cJSON_malloc(256);
    memset(prealloc, 0, 256);
    cJSON_PrintPreallocated(root, prealloc, 256, 1);

    // step 4: Validate & Cleanup
    cJSON_free(pretty);
    cJSON_free(prealloc);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}