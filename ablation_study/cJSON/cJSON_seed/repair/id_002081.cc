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
//<ID> 2081
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *flags = cJSON_CreateArray();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *false_item = cJSON_CreateFalse();
    cJSON *another_bool = cJSON_CreateBool(0);
    cJSON_AddItemToArray(flags, true_item);
    cJSON_AddItemToArray(flags, false_item);
    cJSON_AddItemToArray(flags, another_bool);
    cJSON_AddItemToObject(root, "flags", flags);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 2: Configure
    cJSON *got_flags = cJSON_GetObjectItem(root, "flags");
    cJSON *detached = cJSON_DetachItemFromArray(got_flags, 1);
    cJSON_bool detached_was_true = cJSON_IsTrue(detached);

    // step 3: Operate and Validate
    cJSON_AddItemToArray(got_flags, detached);
    cJSON_AddBoolToObject(root, "detached_was_true", detached_was_true);
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed[0];
    buffer[1] = (char)('0' + (int)detached_was_true);
    buffer[2] = '\0';
    cJSON_free(printed);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}