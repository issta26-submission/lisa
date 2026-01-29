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
//<ID> 1067
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
    cJSON *name = cJSON_CreateString("sample_name");
    cJSON_AddItemToObjectCS(info, "name", name);
    const char *version_str = cJSON_Version();
    cJSON *version_node = cJSON_CreateString(version_str);
    cJSON_AddItemToObjectCS(info, "version", version_node);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObjectCS(root, "items", items);

    // step 2: Configure
    cJSON_AddItemToArray(items, cJSON_CreateNumber(10));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(20));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(30));
    const char *extracted = cJSON_GetStringValue(name);
    cJSON *extracted_copy = cJSON_CreateString(extracted);
    cJSON_AddItemToObjectCS(root, "extracted_name", extracted_copy);

    // step 3: Operate
    char *pretty = cJSON_Print(root);
    char *compact = cJSON_PrintUnformatted(root);
    char *prealloc = (char *)cJSON_malloc(256);
    memset(prealloc, 0, 256);
    cJSON_PrintPreallocated(root, prealloc, 256, 1);

    // step 4: Validate & Cleanup
    cJSON_free(pretty);
    cJSON_free(compact);
    cJSON_free(prealloc);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}