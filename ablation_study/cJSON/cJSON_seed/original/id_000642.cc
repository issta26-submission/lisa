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
//<ID> 642
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *flags = cJSON_AddArrayToObject(root, "flags");
    cJSON *btrue = cJSON_CreateBool(1);
    cJSON *bfalse = cJSON_CreateBool(0);
    cJSON_AddItemToArray(flags, btrue);
    cJSON_AddItemToArray(flags, bfalse);
    cJSON *settings = cJSON_CreateObject();
    cJSON_AddItemToObject(settings, "enabled", cJSON_CreateBool(1));
    cJSON_AddItemToObject(root, "settings", settings);

    // step 2: Configure
    cJSON *first = cJSON_GetArrayItem(flags, 0);
    cJSON_bool first_is_true = cJSON_IsTrue(first);
    cJSON_AddItemToObject(root, "first_is_true", cJSON_CreateBool(first_is_true));

    // step 3: Operate
    cJSON_DeleteItemFromArray(flags, 1);

    // step 4: Validate & Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}