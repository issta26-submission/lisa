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
//<ID> 1268
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "active", cJSON_CreateBool(0));

    // step 2: Configure
    cJSON *replacement = cJSON_CreateBool(1);
    cJSON_ReplaceItemInObject(root, "active", replacement);

    // step 3: Operate & Validate
    cJSON *current = cJSON_GetObjectItem(root, "active");
    cJSON_bool invalid_flag = cJSON_IsInvalid(current);
    cJSON_AddNumberToObject(root, "active_is_invalid", (double)invalid_flag);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}