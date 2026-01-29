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
//<ID> 1264
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
    cJSON *temp_false = cJSON_CreateBool(0);
    cJSON_AddItemToObject(root, "flag", flag_true);
    cJSON_AddItemToObject(root, "temp", temp_false);

    // step 2: Configure
    cJSON *replacement_false = cJSON_CreateBool(0);
    cJSON_ReplaceItemInObject(root, "flag", replacement_false);

    // step 3: Operate & Validate
    cJSON *flag_item = cJSON_GetObjectItem(root, "flag");
    cJSON_bool flag_invalid = cJSON_IsInvalid(flag_item);
    (void)flag_invalid;
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}