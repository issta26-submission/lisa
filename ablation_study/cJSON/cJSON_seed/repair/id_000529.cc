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
//<ID> 529
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("original_name");
    cJSON_AddItemToObject(root, "name", name);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", flag);

    // step 2: Configure
    cJSON *got_name = cJSON_GetObjectItemCaseSensitive(root, "name");
    char *name_val = cJSON_GetStringValue(got_name);
    cJSON *replacement = cJSON_CreateString(name_val);
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(root, name, replacement);
    cJSON *replaced_flag = cJSON_CreateNumber((double)replaced);
    cJSON_AddItemToObject(root, "replaced_flag", replaced_flag);

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}