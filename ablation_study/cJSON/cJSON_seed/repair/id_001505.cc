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
//<ID> 1505
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
    cJSON *active_flag = cJSON_AddFalseToObject(meta, "active");
    cJSON *year = cJSON_CreateNumber(2026.0);
    cJSON_AddItemToObject(root, "year", year);
    cJSON *name = cJSON_CreateString("alpha");
    cJSON_AddItemToObject(meta, "name", name);
    cJSON *shared = cJSON_CreateString("shared_value");
    cJSON_AddItemToObject(meta, "shared", shared);
    cJSON_AddItemReferenceToObject(root, "shared_ref", shared);

    // step 2: Configure
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *got_flag = cJSON_GetObjectItem(got_meta, "active");
    cJSON_bool flag_is_false = cJSON_IsFalse(got_flag);
    cJSON *got_shared = cJSON_GetObjectItem(root, "shared_ref");
    const char *shared_str = cJSON_GetStringValue(got_shared);
    double year_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "year"));

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    size_t buf_len = out_len + 32;
    char *scratch = (char *)cJSON_malloc(buf_len);
    memset(scratch, 0, buf_len);
    scratch[0] = shared_str ? shared_str[0] : 'x';
    scratch[1] = (char)('0' + ((int)year_val % 10));
    scratch[2] = (char)('0' + (int)flag_is_false);
    scratch[3] = out_len ? out[0] : ' ';
    scratch[4] = out_len > 1 ? out[1] : ' ';
    scratch[5] = out_len > 2 ? out[2] : ' ';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}