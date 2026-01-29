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
//<ID> 1269
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *flag_true = cJSON_CreateBool(1);
    cJSON *flag_false = cJSON_CreateBool(0);
    cJSON *meta = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddItemToObject(root, "flagA", flag_true);
    cJSON_AddItemToObject(root, "flagB", flag_false);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "count", 2);
    cJSON *replacement = cJSON_CreateBool(0);
    cJSON_ReplaceItemInObject(root, "flagA", replacement);

    // step 3: Operate & Validate
    cJSON *gotA = cJSON_GetObjectItem(root, "flagA");
    cJSON_bool was_invalid = cJSON_IsInvalid(gotA);
    cJSON_AddBoolToObject(root, "flagA_was_invalid", was_invalid);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}