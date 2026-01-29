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
//<ID> 1965
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

    // step 2: Configure
    cJSON *name = cJSON_CreateString("example_name");
    cJSON_AddItemToObject(meta, "name", name);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(meta, "active", flag);
    cJSON *info = cJSON_CreateObject();
    cJSON_AddItemToObject(meta, "info", info);
    cJSON *desc = cJSON_CreateString("detailed_info");
    cJSON_AddItemToObject(info, "desc", desc);

    // step 3: Operate and Validate
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *got_name = cJSON_GetObjectItem(got_meta, "name");
    cJSON *got_flag = cJSON_GetObjectItem(got_meta, "active");
    const char *name_val = cJSON_GetStringValue(got_name);
    cJSON_bool flag_is_false = cJSON_IsFalse(got_flag);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 1);
    buffer[0] = (char)('0' + ((int)name_val[0] % 10));
    buffer[1] = (char)('0' + (flag_is_false % 10));
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}