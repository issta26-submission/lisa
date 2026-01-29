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
//<ID> 1504
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
    cJSON *name_item = cJSON_CreateString("initial");
    cJSON_AddItemToObject(meta, "name", name_item);
    cJSON *count = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "count", count);
    cJSON *enabled_item = cJSON_AddFalseToObject(root, "enabled");

    // step 2: Configure
    char *set_ret = cJSON_SetValuestring(name_item, "tester");
    cJSON *shared = cJSON_CreateString("shared_value");
    cJSON_AddItemToObject(meta, "shared", shared);
    cJSON_AddItemReferenceToObject(root, "alias", shared);

    // step 3: Operate and Validate
    cJSON *got_enabled = cJSON_GetObjectItem(root, "enabled");
    cJSON_bool was_false = cJSON_IsFalse(got_enabled);
    cJSON *got_alias = cJSON_GetObjectItem(root, "alias");
    const char *alias_str = cJSON_GetStringValue(got_alias);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    size_t buf_len = out_len + 32;
    char *scratch = (char *)cJSON_malloc(buf_len);
    memset(scratch, 0, buf_len);
    scratch[0] = (char)('0' + (int)was_false);
    scratch[1] = alias_str ? alias_str[0] : 'x';
    scratch[2] = set_ret ? set_ret[0] : 'y';
    scratch[3] = out_len ? out[0] : ' ';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}