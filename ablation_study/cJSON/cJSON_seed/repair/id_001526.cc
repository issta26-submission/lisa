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
//<ID> 1526
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "name", "initial_name");
    cJSON_AddStringToObject(root, "replace_me", "old_value");
    cJSON_AddStringToObject(root, "author", "tester");

    // step 2: Configure
    cJSON *new_name = cJSON_CreateString("new_name_v2");
    cJSON_bool replaced = cJSON_ReplaceItemInObject(root, "name", new_name);
    cJSON *replaced_item = cJSON_GetObjectItemCaseSensitive(root, "replace_me");
    cJSON_bool replaced_item_is_string = cJSON_IsString(replaced_item);

    // step 3: Operate and Validate
    const char *version = cJSON_Version();
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    size_t version_len = version ? strlen(version) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + version_len + 16);
    memset(scratch, 0, out_len + version_len + 16);
    const char *rep_str = cJSON_GetStringValue(replaced_item);
    scratch[0] = version_len ? version[0] : 'v';
    scratch[1] = rep_str ? rep_str[0] : 'x';
    scratch[2] = replaced ? '1' : '0';
    scratch[3] = replaced_item_is_string ? 'S' : 'N';
    scratch[4] = out_len ? out[0] : ' ';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}