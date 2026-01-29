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
//<ID> 239
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *flags = cJSON_CreateArray();
    cJSON *flag_false = cJSON_CreateFalse();

    // step 2: Configure
    cJSON_InsertItemInArray(flags, 0, flag_false);
    cJSON_AddItemToObject(root, "flags", flags);
    cJSON *meta = cJSON_CreateObject();
    cJSON *meta_disabled = cJSON_CreateFalse();
    cJSON_AddItemToObject(meta, "disabled", meta_disabled);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate and Validate
    cJSON *retrieved_flags = cJSON_GetObjectItem(root, "flags");
    cJSON *retrieved_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *retrieved_disabled = cJSON_GetObjectItem(retrieved_meta, "disabled");
    cJSON_bool disabled_is_false = cJSON_IsFalse(retrieved_disabled);
    cJSON_AddBoolToObject(root, "meta_disabled_flag", disabled_is_false);
    cJSON_AddNumberToObject(root, "flags_is_array", (double)cJSON_IsArray(retrieved_flags));
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}