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
//<ID> 872
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "child", child);
    cJSON *name = cJSON_CreateString("Alice");
    cJSON_AddItemToObjectCS(child, "name", name);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObjectCS(child, "maybe_null", maybe_null);

    // step 2: Configure
    const char *ver = cJSON_Version();
    cJSON *version_item = cJSON_CreateString(ver);
    cJSON_AddItemToObjectCS(root, "version", version_item);
    const char *retrieved_name = cJSON_GetStringValue(cJSON_GetObjectItem(child, "name"));
    cJSON *name_ref = cJSON_CreateStringReference(retrieved_name);
    cJSON_AddItemToObjectCS(root, "name_ref", name_ref);

    // step 3: Operate
    cJSON *dup = cJSON_Duplicate(root, 1);
    char *serialized = cJSON_PrintUnformatted(dup);

    // step 4: Validate & Cleanup
    cJSON_free(serialized);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}