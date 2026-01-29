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
//<ID> 1993
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
    cJSON *name = cJSON_CreateString("original_name");
    cJSON_AddItemToObject(child, "name", name);

    // step 2: Configure
    cJSON_AddFalseToObject(root, "enabled");
    cJSON_AddNullToObject(root, "optional");
    cJSON *ver_item = cJSON_CreateString("unknown_version");
    cJSON_AddItemToObject(root, "lib_version", ver_item);
    cJSON_SetValuestring(ver_item, cJSON_Version());

    // step 3: Operate and Validate
    cJSON *id = cJSON_CreateNumber(314.0);
    cJSON_AddItemToObject(child, "id", id);
    const char *ver_str = cJSON_GetStringValue(ver_item);
    cJSON *got_child = cJSON_GetObjectItem(root, "child");
    cJSON *got_name = cJSON_GetObjectItem(got_child, "name");
    const char *name_str = cJSON_GetStringValue(got_name);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    buffer[0] = (char)('0' + (printed % 10));
    buffer[1] = (ver_str && ver_str[0]) ? ver_str[0] : 'v';
    buffer[2] = (name_str && name_str[0]) ? name_str[0] : 'n';
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}