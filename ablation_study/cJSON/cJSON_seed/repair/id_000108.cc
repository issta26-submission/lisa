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
//<ID> 108
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddNumberToObject(meta, "version", 1.23);
    cJSON *name = cJSON_CreateString("initial");
    cJSON_AddItemToObject(root, "name", name);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", flag);

    // step 2: Configure
    char *name_str = cJSON_GetStringValue(name);
    cJSON_AddNumberToObject(root, "name_first_char_code", (double)(unsigned char)name_str[0]);
    cJSON_AddNumberToObject(root, "meta_version_times_10", 1.23 * 10.0);

    // step 3: Operate and Validate
    cJSON_bool was_false = cJSON_IsFalse(flag);
    cJSON_AddNumberToObject(root, "enabled_is_false", (double)was_false);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}