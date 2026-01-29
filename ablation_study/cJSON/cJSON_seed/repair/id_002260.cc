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
//<ID> 2260
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
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    cJSON *orig = cJSON_CreateString("original_label");
    cJSON_AddItemToObject(child, "label", orig);
    cJSON *num = cJSON_CreateNumber(123.0);
    cJSON_AddItemToObject(child, "count", num);

    // step 3: Operate and Validate
    cJSON *dup = cJSON_Duplicate(child, 1);
    cJSON_AddItemToObject(root, "child_copy", dup);
    const char *ver = cJSON_Version();
    cJSON *ver_item = cJSON_CreateString(ver);
    cJSON_AddItemToObject(root, "version", ver_item);
    cJSON *replacement = cJSON_CreateString("replaced_label");
    cJSON_ReplaceItemViaPointer(child, orig, replacement);
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (ver && ver[0]) ? ver[0] : '\0';
    cJSON_free(buffer);
    cJSON_free(json);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}