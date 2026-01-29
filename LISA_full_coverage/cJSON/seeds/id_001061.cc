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
//<ID> 1061
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *version_item = cJSON_CreateString(cJSON_Version());
    cJSON_AddItemToObjectCS(root, "version", version_item);
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *name_item = cJSON_CreateString("example_project");
    cJSON_AddItemToObjectCS(meta, "name", name_item);

    // step 2: Configure
    cJSON *count_item = cJSON_CreateNumber(3);
    cJSON_AddItemToObjectCS(root, "count", count_item);
    cJSON_AddItemToObjectCS(root, "enabled", cJSON_CreateTrue());
    const char *ver_str = cJSON_GetStringValue(version_item);
    cJSON *ver_copy = cJSON_CreateString(ver_str);
    cJSON_AddItemToObjectCS(meta, "version_copy", ver_copy);

    // step 3: Operate
    char *pretty = cJSON_Print(root);
    char *compact = cJSON_PrintUnformatted(root);
    char *prealloc = (char *)cJSON_malloc(512);
    memset(prealloc, 0, 512);
    cJSON_PrintPreallocated(root, prealloc, 512, 1);

    // step 4: Validate & Cleanup
    cJSON_free(pretty);
    cJSON_free(compact);
    cJSON_free(prealloc);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}