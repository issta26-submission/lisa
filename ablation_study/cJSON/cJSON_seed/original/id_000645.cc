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
//<ID> 645
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
    cJSON *b_true = cJSON_CreateBool(1);
    cJSON *b_false = cJSON_CreateBool(0);
    cJSON_AddItemToArray(flags, b_true);
    cJSON_AddItemToArray(flags, b_false);
    cJSON_AddItemToObject(root, "status", cJSON_CreateBool(1));

    // step 3: Operate
    int size_before = cJSON_GetArraySize(flags);
    cJSON_DeleteItemFromArray(flags, 0);
    int size_after = cJSON_GetArraySize(flags);
    cJSON_bool still_has_flags = (size_after > 0);
    cJSON_AddItemToObject(root, "has_flags", cJSON_CreateBool(still_has_flags));
    (void)size_before;
    (void)size_after;

    // step 4: Validate & Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}