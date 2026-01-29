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
//<ID> 1529
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
    const char *version_str = cJSON_Version();
    cJSON_AddStringToObject(root, "lib_version", version_str);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "author", "fuzz_tester");

    // step 2: Configure
    cJSON *replacement_name = cJSON_CreateString("replaced_name");
    cJSON_bool replaced_flag = cJSON_ReplaceItemInObject(root, "name", replacement_name);
    cJSON_AddNumberToObject(root, "replaced_flag", (double)replaced_flag);

    // step 3: Operate and Validate
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(root, "name");
    cJSON *ver_item = cJSON_GetObjectItemCaseSensitive(root, "lib_version");
    const char *name_val = cJSON_GetStringValue(name_item);
    const char *ver_val = cJSON_GetStringValue(ver_item);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 32);
    memset(scratch, 0, out_len + 32);
    scratch[0] = name_val && name_val[0] ? name_val[0] : 'x';
    scratch[1] = ver_val && ver_val[0] ? ver_val[0] : 'y';
    scratch[2] = out_len ? out[0] : ' ';
    scratch[3] = (char)('0' + (int)replaced_flag);
    scratch[4] = (char)('#');

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}