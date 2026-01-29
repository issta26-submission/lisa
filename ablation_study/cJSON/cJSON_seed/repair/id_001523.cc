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
//<ID> 1523
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON *mode_orig = cJSON_AddStringToObject(settings, "mode", "initial");
    const char *lib_ver = cJSON_Version();
    cJSON *ver_item = cJSON_AddStringToObject(root, "lib_version", lib_ver);

    // step 2: Configure
    cJSON *mode_new = cJSON_CreateString("updated");
    cJSON_bool replaced = cJSON_ReplaceItemInObject(settings, "mode", mode_new);
    cJSON *mode_item = cJSON_GetObjectItemCaseSensitive(settings, "mode");
    cJSON *libver_item = cJSON_GetObjectItemCaseSensitive(root, "lib_version");

    // step 3: Operate and Validate
    cJSON *repl_flag = cJSON_CreateNumber((double)replaced);
    cJSON_AddItemToObject(root, "mode_replaced", repl_flag);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    const char *mode_str = mode_item ? cJSON_GetStringValue(mode_item) : NULL;
    const char *ver_str = libver_item ? cJSON_GetStringValue(libver_item) : NULL;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = mode_str ? mode_str[0] : 'x';
    scratch[1] = ver_str ? ver_str[0] : 'v';
    scratch[2] = (char)('0' + (int)replaced);
    scratch[3] = out_len ? out[0] : ' ';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}