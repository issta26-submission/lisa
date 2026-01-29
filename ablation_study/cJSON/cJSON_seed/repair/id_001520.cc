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
//<ID> 1520
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
    cJSON *initial_name = cJSON_AddStringToObject(meta, "name", "initial_name");
    cJSON *status = cJSON_AddStringToObject(root, "status", "ok");
    cJSON *replacement_name = cJSON_CreateString("replaced_name");
    cJSON_bool replace_ok = cJSON_ReplaceItemInObject(meta, "name", replacement_name);
    (void)replace_ok;

    // step 3: Operate and Validate
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(meta, "name");
    const char *name_val = cJSON_GetStringValue(name_item);
    const char *version_str = cJSON_Version();
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 32);
    memset(scratch, 0, out_len + 32);
    scratch[0] = name_val ? name_val[0] : 'x';
    scratch[1] = version_str ? version_str[0] : 'v';
    scratch[2] = out_len ? out[0] : ' ';
    scratch[3] = status && status->valuestring ? status->valuestring[0] : 's';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}