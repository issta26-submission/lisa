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
//<ID> 1501
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
    cJSON *name = cJSON_CreateString("initial");
    cJSON_AddItemToObject(meta, "name", name);
    cJSON *count = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(root, "count", count);
    cJSON *active = cJSON_AddFalseToObject(root, "active");

    // step 2: Configure
    cJSON *retrieved_meta = cJSON_GetObjectItem(root, "meta");
    cJSON_AddItemReferenceToObject(root, "name_alias", name);

    // step 3: Operate and Validate
    cJSON *active_item = cJSON_GetObjectItem(root, "active");
    cJSON_bool is_false = cJSON_IsFalse(active_item);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    size_t buf_len = out_len + 32;
    char *scratch = (char *)cJSON_malloc(buf_len);
    memset(scratch, 0, buf_len);
    scratch[0] = (char)('0' + (int)is_false);
    scratch[1] = out_len ? out[0] : ' ';
    const char *name_str = cJSON_GetStringValue(name);
    scratch[2] = name_str ? name_str[0] : 'x';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}