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
//<ID> 1261
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *initial_flag = cJSON_CreateBool(1);

    // step 2: Configure
    cJSON_AddItemToObject(root, "status", initial_flag);
    cJSON *replacement_flag = cJSON_CreateBool(0);
    cJSON_bool replaced = cJSON_ReplaceItemInObject(root, "status", replacement_flag);

    // step 3: Operate & Validate
    cJSON *current = cJSON_GetObjectItem(root, "status");
    cJSON_bool is_invalid = cJSON_IsInvalid(current);
    cJSON_AddNumberToObject(root, "status_is_invalid", (double)is_invalid);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}