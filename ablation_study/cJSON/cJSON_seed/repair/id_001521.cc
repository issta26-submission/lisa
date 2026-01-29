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
//<ID> 1521
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
    cJSON *name_item = cJSON_AddStringToObject(meta, "name", "fuzz_tester");
    cJSON *status_item = cJSON_AddStringToObject(root, "status", "initial");
    cJSON *note_item = cJSON_AddStringToObject(meta, "note", "original");

    // step 2: Configure
    cJSON *new_status = cJSON_CreateString("ready");
    cJSON_ReplaceItemInObject(root, "status", new_status);
    cJSON_AddStringToObject(meta, "added", "yes");

    // step 3: Operate and Validate
    const char *version = cJSON_Version();
    cJSON *got_status = cJSON_GetObjectItemCaseSensitive(root, "status");
    cJSON *got_name = cJSON_GetObjectItemCaseSensitive(meta, "name");
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 32);
    memset(scratch, 0, out_len + 32);
    scratch[0] = version ? version[0] : 'v';
    const char *s_stat = cJSON_GetStringValue(got_status);
    const char *s_name = cJSON_GetStringValue(got_name);
    scratch[1] = s_stat ? s_stat[0] : 's';
    scratch[2] = s_name ? s_name[0] : 'n';
    scratch[3] = out_len ? out[0] : ' ';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}