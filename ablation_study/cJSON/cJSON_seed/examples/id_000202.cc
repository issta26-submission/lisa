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
//<ID> 202
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *orig = cJSON_CreateString("original_value");
    cJSON_AddItemToObject(root, "key", orig);
    cJSON *nullitem = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "nullable", nullitem);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateStringReference("replacement_ref");
    cJSON_bool replaced_flag = cJSON_ReplaceItemViaPointer(root, orig, replacement);

    // step 3: Operate & Validate
    cJSON_bool is_null_flag = cJSON_IsNull(nullitem);
    cJSON_AddNumberToObject(root, "replaced_flag", replaced_flag ? 1.0 : 0.0);
    cJSON_AddNumberToObject(root, "is_null_flag", is_null_flag ? 1.0 : 0.0);
    char *serialized = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(serialized);
    cJSON_Delete(root);

    return 66;
    // API sequence test completed successfully
}