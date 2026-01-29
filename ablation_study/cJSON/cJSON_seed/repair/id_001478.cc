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
//<ID> 1478
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
    cJSON *name_item = cJSON_CreateString("example-name");
    cJSON_AddItemToObject(meta, "name", name_item);

    // step 2: Configure
    cJSON *true_flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "is_active", true_flag);
    cJSON *false_flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "is_deprecated", false_flag);
    cJSON *num_item = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObject(root, "count", num_item);
    double new_count = cJSON_SetNumberHelper(num_item, 42.0);

    // step 3: Operate and Validate
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *got_name = cJSON_GetObjectItem(got_meta, "name");
    const char *name_str = cJSON_GetStringValue(got_name);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 8);
    memset(scratch, 0, out_len + 8);
    scratch[0] = name_str ? name_str[0] : 'n';
    scratch[1] = (char)('0' + ((int)new_count % 10));
    scratch[2] = out_len ? out[0] : ' ';
    scratch[3] = (char)('A');

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}