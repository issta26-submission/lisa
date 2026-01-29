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
//<ID> 646
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *flags = cJSON_AddArrayToObject(root, "flags");
    cJSON *flag_true = cJSON_CreateBool(1);
    cJSON *flag_false = cJSON_CreateBool(0);
    cJSON_AddItemToArray(flags, flag_true);
    cJSON_AddItemToArray(flags, flag_false);
    cJSON *top_flag = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "top_flag", top_flag);

    // step 3: Operate
    cJSON_DeleteItemFromArray(flags, 0);
    cJSON *first_after_delete = cJSON_GetArrayItem(flags, 0);
    cJSON_bool first_is_true = cJSON_IsTrue(first_after_delete);
    int flags_count = cJSON_GetArraySize(flags);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);

    // step 4: Validate & Cleanup
    (void)first_is_true;
    (void)flags_count;
    (void)first_after_delete;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}