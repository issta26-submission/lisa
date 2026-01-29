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
//<ID> 871
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
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObjectCS(child, "maybe", null_item);
    cJSON *name = cJSON_CreateString("original_name");
    cJSON_AddItemToObjectCS(child, "name", name);
    const char *ver = cJSON_Version();
    cJSON *ver_item = cJSON_CreateString(ver);
    cJSON_AddItemToObjectCS(root, "version", ver_item);

    // step 2: Configure
    char *name_str = cJSON_GetStringValue(cJSON_GetObjectItem(child, "name"));
    char outbuf[256];
    memset(outbuf, 0, sizeof(outbuf));
    cJSON_bool printed = cJSON_PrintPreallocated(root, outbuf, (int)sizeof(outbuf), 0);
    (void)printed;
    char *raw = cJSON_PrintUnformatted(root);

    // step 3: Operate
    cJSON *dup_child = cJSON_Duplicate(child, 1);
    cJSON_AddItemToObjectCS(root, "child_copy", dup_child);
    cJSON *new_ver = cJSON_CreateString("v2.0");
    cJSON_ReplaceItemInObject(root, "version", new_ver);
    char *version_str = cJSON_GetStringValue(cJSON_GetObjectItem(root, "version"));

    // step 4: Validate & Cleanup
    (void)name_str;
    (void)version_str;
    cJSON_free(raw);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}