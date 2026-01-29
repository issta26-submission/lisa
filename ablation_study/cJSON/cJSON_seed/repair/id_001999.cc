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
//<ID> 1999
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
    cJSON *name = cJSON_CreateString("initial_name");
    cJSON_AddItemToObject(child, "name", name);
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    cJSON_AddFalseToObject(root, "enabled");
    cJSON_AddNullToObject(root, "optional");
    cJSON *description = cJSON_CreateString("old_description");
    cJSON_AddItemToObject(root, "description", description);
    cJSON_SetValuestring(description, "new_description");
    const char *version = cJSON_Version();

    // step 3: Operate and Validate
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 1);
    cJSON *got_desc = cJSON_GetObjectItem(root, "description");
    char *got_desc_str = cJSON_GetStringValue(got_desc);
    cJSON *got_enabled = cJSON_GetObjectItem(root, "enabled");
    cJSON_bool was_false = cJSON_IsFalse(got_enabled);
    buffer[0] = version[0];
    buffer[1] = got_desc_str[0];
    buffer[2] = (char)('0' + ((int)was_false % 10));
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}