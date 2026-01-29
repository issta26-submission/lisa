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
//<ID> 2262
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
    cJSON *name = cJSON_CreateString("device42");
    cJSON_AddItemToObject(config, "name", name);
    cJSON *count = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(config, "count", count);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    const char *ver = cJSON_Version();
    cJSON *verstr = cJSON_CreateString(ver);
    cJSON_AddItemToObject(meta, "lib", verstr);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(child, "value", cJSON_CreateNumber(100.0));
    cJSON_AddItemToObject(root, "child", child);

    // step 3: Operate and Validate
    cJSON *dup_child = cJSON_Duplicate(child, 1);
    cJSON_AddItemToObject(root, "child_copy", dup_child);
    cJSON *new_child = cJSON_CreateObject();
    cJSON_AddItemToObject(new_child, "value", cJSON_CreateNumber(200.0));
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(root, child, new_child);
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (char)(replaced ? '1' : '0');
    cJSON_free(buffer);
    cJSON_free(json);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}