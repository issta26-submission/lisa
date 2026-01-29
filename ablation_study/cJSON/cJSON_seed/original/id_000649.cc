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
//<ID> 649
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize & Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *flags = cJSON_AddArrayToObject(root, "flags");
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *b_true = cJSON_CreateBool(1);
    cJSON *b_false = cJSON_CreateBool(0);
    cJSON_AddItemToArray(flags, b_true);
    cJSON_AddItemToArray(flags, b_false);

    // step 2: Operate
    int size_before = cJSON_GetArraySize(flags);
    cJSON_DeleteItemFromArray(flags, 1);

    // step 3: Validate
    int size_after = cJSON_GetArraySize(flags);
    cJSON_AddNumberToObject(meta, "size_before", (double)size_before);
    cJSON_AddNumberToObject(meta, "size_after", (double)size_after);
    cJSON *first = cJSON_GetArrayItem(flags, 0);
    cJSON_bool first_is_true = cJSON_IsTrue(first);
    cJSON_AddItemToObject(meta, "first_is_true", cJSON_CreateBool(first_is_true));
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}