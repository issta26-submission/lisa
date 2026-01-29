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
//<ID> 1265
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
    cJSON_AddItemToObject(root, "flag", initial_flag);
    cJSON *sub = cJSON_CreateObject();
    cJSON *inner_flag = cJSON_CreateBool(0);
    cJSON_AddItemToObject(sub, "inner", inner_flag);
    cJSON_AddItemToObject(root, "sub", sub);

    // step 2: Configure
    cJSON *replacement_flag = cJSON_CreateBool(0);
    cJSON_ReplaceItemInObject(root, "flag", replacement_flag);

    // step 3: Operate & Validate
    cJSON *got_flag = cJSON_GetObjectItem(root, "flag");
    cJSON_bool flag_invalid = cJSON_IsInvalid(got_flag);
    cJSON *invalid_marker = cJSON_CreateBool(flag_invalid);
    cJSON_AddItemToObject(root, "flag_was_invalid", invalid_marker);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}