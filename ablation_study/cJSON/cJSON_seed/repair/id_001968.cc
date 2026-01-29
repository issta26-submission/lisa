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
//<ID> 1968
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
    cJSON *name = cJSON_CreateString("test_name");
    cJSON_AddItemToObject(config, "name", name);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(config, "ok", flag);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *desc = cJSON_CreateString("configuration example");
    cJSON_AddItemToObject(meta, "description", desc);

    // step 3: Operate and Validate
    cJSON *got_config = cJSON_GetObjectItem(root, "config");
    cJSON *got_name = cJSON_GetObjectItem(got_config, "name");
    const char *name_str = cJSON_GetStringValue(got_name);
    cJSON *got_flag = cJSON_GetObjectItem(got_config, "ok");
    cJSON_bool flag_is_false = cJSON_IsFalse(got_flag);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 0);
    buffer[0] = (char)((name_str && name_str[0]) ? name_str[0] : '0');
    buffer[1] = (char)('0' + (flag_is_false % 10));
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}