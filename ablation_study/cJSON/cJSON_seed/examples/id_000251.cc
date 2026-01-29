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
//<ID> 251
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
    const char * const tags_list[] = { "alpha", "beta", "gamma" };
    cJSON *tags = cJSON_CreateStringArray(tags_list, 3);
    cJSON *version = cJSON_CreateNumber(1.0);
    cJSON *value = cJSON_CreateNumber(2.0);

    // step 2: Configure
    cJSON_AddItemToObjectCS(meta, "tags", tags);
    cJSON_AddItemToObjectCS(meta, "version", version);
    cJSON_AddItemToObjectCS(root, "meta", meta);
    cJSON_AddItemToObjectCS(root, "value", value);

    // step 3: Operate & Validate
    cJSON_bool has_meta = cJSON_HasObjectItem(root, "meta");
    cJSON *has_meta_num = cJSON_CreateNumber((double)has_meta);
    cJSON_AddItemToObjectCS(root, "has_meta", has_meta_num);
    cJSON *old_value_ptr = cJSON_GetObjectItem(root, "value");
    cJSON *replacement = cJSON_CreateNumber(42.0);
    cJSON_bool replace_result = cJSON_ReplaceItemViaPointer(root, old_value_ptr, replacement);
    cJSON *replace_result_num = cJSON_CreateNumber((double)replace_result);
    cJSON_AddItemToObjectCS(root, "replace_ok", replace_result_num);
    cJSON *new_value_ptr = cJSON_GetObjectItem(root, "value");
    double new_value = cJSON_GetNumberValue(new_value_ptr);
    cJSON *value_copy = cJSON_CreateNumber(new_value);
    cJSON_AddItemToObjectCS(root, "value_copy", value_copy);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}