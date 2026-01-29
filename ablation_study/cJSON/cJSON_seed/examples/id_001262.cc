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
//<ID> 1262
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
    cJSON_AddItemToObject(root, "flag", bool_true);

    // step 2: Configure
    cJSON *bool_false = cJSON_CreateBool(0);
    cJSON_ReplaceItemInObject(root, "flag", bool_false);
    cJSON *flag = cJSON_GetObjectItem(root, "flag");
    cJSON_bool invalid_flag = cJSON_IsInvalid(flag);
    cJSON *invalid_marker = cJSON_CreateNumber((double)invalid_flag);
    cJSON_AddItemToObject(root, "is_invalid", invalid_marker);

    // step 3: Operate & Validate
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}