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
//<ID> 1522
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
    const char *version_const = cJSON_Version();
    cJSON_AddStringToObject(meta, "library_version", version_const);
    cJSON *status = cJSON_AddStringToObject(root, "status", "initial");

    // step 2: Configure
    cJSON *replacement_status = cJSON_CreateString("replaced");
    cJSON_bool replaced_flag = cJSON_ReplaceItemInObject(root, "status", replacement_status);

    // step 3: Operate and Validate
    cJSON *status_item = cJSON_GetObjectItemCaseSensitive(root, "status");
    const char *status_text = cJSON_GetStringValue(status_item);
    const char *meta_version_text = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(root, "meta")->child);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 24);
    memset(scratch, 0, out_len + 24);
    scratch[0] = version_const ? version_const[0] : 'v';
    scratch[1] = status_text ? status_text[0] : 's';
    scratch[2] = (char)('0' + (int)replaced_flag);
    scratch[3] = out_len ? out[0] : ' ';
    scratch[4] = meta_version_text ? meta_version_text[0] : 'm';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}