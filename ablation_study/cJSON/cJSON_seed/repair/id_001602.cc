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
//<ID> 1602
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *bool_item = cJSON_CreateBool(0);
    const char *version_str = cJSON_Version();
    cJSON *version_item = cJSON_CreateString(version_str);
    cJSON_AddItemToObject(root, "active", true_item);
    cJSON_AddItemToObject(root, "enabled", bool_item);
    cJSON_AddItemToObject(root, "version", version_item);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *meta_flag = cJSON_CreateBool(1);
    cJSON_AddItemToObject(meta, "meta_flag", meta_flag);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char buffer[128];
    memset(buffer, 0, sizeof(buffer));
    size_t printed_len = printed ? strlen(printed) : 0;
    size_t cap = sizeof(buffer) - 1;
    size_t copy_len = printed_len < cap ? printed_len : cap;
    memcpy(buffer, printed, copy_len);
    cJSON *got_version = cJSON_GetObjectItem(root, "version");
    const char *got_version_str = cJSON_GetStringValue(got_version);
    buffer[127] = got_version_str ? got_version_str[0] : '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}