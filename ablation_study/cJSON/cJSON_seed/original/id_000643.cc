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
//<ID> 643
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    cJSON *flags = cJSON_AddArrayToObject(root, "flags");

    // step 2: Configure
    cJSON *flag_true = cJSON_CreateBool(1);
    cJSON *flag_false = cJSON_CreateBool(0);
    cJSON_AddItemToArray(flags, flag_true);
    cJSON_AddItemToArray(flags, flag_false);

    // step 3: Operate
    int size_before = cJSON_GetArraySize(flags);
    cJSON_DeleteItemFromArray(flags, 0);
    cJSON *flag_extra = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "extra_flag", flag_extra);
    int size_after = cJSON_GetArraySize(flags);

    // step 4: Validate & Cleanup
    char *out = cJSON_PrintUnformatted(root);
    (void)size_before;
    (void)size_after;
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}