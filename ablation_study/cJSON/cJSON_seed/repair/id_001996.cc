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
//<ID> 1996
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
    cJSON *name_item = cJSON_CreateString("orig_name");
    cJSON_AddItemToObject(child, "name", name_item);

    // step 2: Configure
    cJSON_AddFalseToObject(root, "ok");
    cJSON_AddNullToObject(root, "maybe");
    const char *version = cJSON_Version();

    // step 3: Operate and Validate
    cJSON_SetValuestring(name_item, "new_name");
    cJSON *got_name = cJSON_GetObjectItem(child, "name");
    const char *name_str = cJSON_GetStringValue(got_name);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    buffer[0] = name_str[0];
    buffer[1] = version[0];
    buffer[2] = (char)('0' + (int)printed % 10);
    buffer[3] = '\0';
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}