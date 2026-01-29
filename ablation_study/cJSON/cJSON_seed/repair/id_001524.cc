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
//<ID> 1524
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *info = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "info", info);
    cJSON *name_item = cJSON_AddStringToObject(info, "name", "api_tester");
    cJSON *version_item = cJSON_AddStringToObject(info, "version", version);

    // step 2: Configure
    cJSON *replacement_name = cJSON_CreateString("replaced_name");
    cJSON_bool replaced = cJSON_ReplaceItemInObject(info, "name", replacement_name);
    (void)replaced;

    // step 3: Operate and Validate
    cJSON *got_name_item = cJSON_GetObjectItemCaseSensitive(info, "name");
    const char *got_name = cJSON_GetStringValue(got_name_item);
    const char *got_version = cJSON_GetStringValue(version_item);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = got_name ? got_name[0] : 'x';
    scratch[1] = got_version ? got_version[0] : 'y';
    scratch[2] = out_len ? out[0] : ' ';
    scratch[3] = (char)('0' + (int)cJSON_IsString(replacement_name));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}