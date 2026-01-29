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
//<ID> 2082
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize & Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *flags = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "flags", flags);
    cJSON *active_item = cJSON_AddBoolToObject(root, "active", 1);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(flags, true_item);
    cJSON_AddItemToArray(flags, false_item);

    // step 2: Operate
    cJSON *first_flag = cJSON_GetArrayItem(flags, 0);
    cJSON_bool is_active_true = cJSON_IsTrue(active_item);
    cJSON_bool is_first_true = cJSON_IsTrue(first_flag);
    cJSON *detached = cJSON_DetachItemFromArray(flags, 1);
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed[0];
    buffer[1] = (char)('0' + (int)is_active_true);
    buffer[2] = (char)('0' + (int)is_first_true);
    buffer[3] = (char)('A' + (char)(detached->type & 0x0F));

    // step 3: Validate
    cJSON *root_copy = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, root_copy, 1);
    buffer[4] = (char)('0' + (int)equal);
    cJSON_free(printed);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root_copy);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}