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
//<ID> 1997
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *name = cJSON_CreateString("initial");
    cJSON_AddItemToObject(meta, "name", name);

    // step 2: Configure
    cJSON *flag = cJSON_AddFalseToObject(root, "ok");
    cJSON *token = cJSON_AddNullToObject(root, "token");
    const char *version_str = cJSON_Version();
    cJSON *version_item = cJSON_CreateString(version_str);
    cJSON_AddItemToObject(root, "version", version_item);
    char *set_res = cJSON_SetValuestring(name, "updated_name");

    // step 3: Operate and Validate
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    cJSON *got_name = cJSON_GetObjectItem(meta, "name");
    const char *got_name_str = cJSON_GetStringValue(got_name);
    cJSON *got_version = cJSON_GetObjectItem(root, "version");
    const char *got_version_str = cJSON_GetStringValue(got_version);
    buffer[0] = (got_name_str && got_name_str[0]) ? got_name_str[0] : 'x';
    buffer[1] = (got_version_str && got_version_str[0]) ? got_version_str[0] : 'v';
    buffer[2] = (char)('0' + ((int)printed_ok % 10));
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}