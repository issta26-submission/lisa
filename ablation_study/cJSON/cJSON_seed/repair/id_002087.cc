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
//<ID> 2087
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *obj_flag = cJSON_AddBoolToObject(root, "active", 1);

    // step 2: Configure
    cJSON *arr_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, arr_item);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemFromArray(arr, 0);
    cJSON_bool detached_is_true = cJSON_IsTrue(detached);
    cJSON *got_active = cJSON_GetObjectItem(root, "active");
    cJSON_bool active_is_true = cJSON_IsTrue(got_active);
    const int buflen = 8;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (char)('0' + (int)detached_is_true);
    buffer[1] = (char)('0' + (int)active_is_true);
    buffer[2] = '\0';
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}