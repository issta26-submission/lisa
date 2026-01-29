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
//<ID> 1977
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
    cJSON *orig_name = cJSON_CreateString("original_name");
    cJSON_AddItemToObject(child, "name", orig_name);

    // step 2: Configure
    cJSON *status = cJSON_CreateString("ready");
    cJSON_AddItemToObject(root, "status", status);
    cJSON *note = cJSON_CreateString("configured");
    cJSON_AddItemToObject(root, "note", note);

    // step 3: Operate and Validate
    cJSON *replacement_ref = cJSON_CreateStringReference("replaced_name");
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(child, orig_name, replacement_ref);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 0);
    buffer[0] = (char)('0' + (replaced % 10));
    buffer[1] = '\0';
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}