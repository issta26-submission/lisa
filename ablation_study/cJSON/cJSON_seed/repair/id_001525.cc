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
//<ID> 1525
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
    cJSON *name_item = cJSON_AddStringToObject(meta, "name", "initial_name");
    cJSON *version_item = cJSON_AddStringToObject(meta, "version", cJSON_Version());
    cJSON *status_item = cJSON_AddStringToObject(root, "status", "ok");

    // step 2: Configure
    cJSON *new_name = cJSON_CreateString("replaced_name");
    cJSON_bool replaced = cJSON_ReplaceItemInObject(meta, "name", new_name);
    cJSON *retrieved = cJSON_GetObjectItemCaseSensitive(meta, "name");
    const char *retrieved_str = cJSON_GetStringValue(retrieved);
    cJSON *ver_retrieved = cJSON_GetObjectItemCaseSensitive(meta, "version");
    const char *ver_str = cJSON_GetStringValue(ver_retrieved);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 32);
    memset(scratch, 0, out_len + 32);
    scratch[0] = retrieved_str ? retrieved_str[0] : 'x';
    scratch[1] = ver_str ? ver_str[0] : 'x';
    scratch[2] = (char)('0' + (int)replaced);
    scratch[3] = out_len ? out[0] : ' ';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}