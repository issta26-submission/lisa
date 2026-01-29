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
//<ID> 1267
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *bool_true = cJSON_CreateBool(1);
    cJSON *bool_false = cJSON_CreateBool(0);

    // step 2: Configure
    cJSON_AddItemToObject(root, "status", bool_true);
    cJSON_ReplaceItemInObject(root, "status", bool_false);

    // step 3: Operate & Validate
    cJSON *status_item = cJSON_GetObjectItem(root, "status");
    cJSON_bool invalid_flag = cJSON_IsInvalid(status_item);
    cJSON_AddNumberToObject(root, "status_is_invalid", (double)invalid_flag);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}