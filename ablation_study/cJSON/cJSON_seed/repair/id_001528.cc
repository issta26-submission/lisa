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
//<ID> 1528
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
    cJSON *name_item = cJSON_AddStringToObject(meta, "name", "fuzz_agent");
    cJSON *status_item = cJSON_AddStringToObject(meta, "status", "legacy");
    const char *version_cstr = cJSON_Version();
    cJSON *version_item = cJSON_AddStringToObject(root, "lib_version", version_cstr);

    // step 2: Configure
    cJSON *new_status = cJSON_CreateString("active");
    cJSON_bool replaced_flag = cJSON_ReplaceItemInObject(meta, "status", new_status);
    cJSON *replaced_num = cJSON_CreateNumber((double)replaced_flag);
    cJSON_AddItemToObject(root, "status_replaced", replaced_num);

    // step 3: Operate and Validate
    cJSON *got_status = cJSON_GetObjectItemCaseSensitive(meta, "status");
    cJSON *got_name = cJSON_GetObjectItemCaseSensitive(meta, "name");
    cJSON *got_version = cJSON_GetObjectItemCaseSensitive(root, "lib_version");
    const char *got_name_str = cJSON_GetStringValue(got_name);
    const char *got_status_str = cJSON_GetStringValue(got_status);
    const char *got_version_str = cJSON_GetStringValue(got_version);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 32);
    memset(scratch, 0, out_len + 32);
    scratch[0] = got_name_str ? got_name_str[0] : 'N';
    scratch[1] = got_status_str ? got_status_str[0] : 'S';
    scratch[2] = got_version_str ? got_version_str[0] : 'V';
    scratch[3] = out_len ? out[0] : ' ';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}