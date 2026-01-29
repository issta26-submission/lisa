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
//<ID> 688
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToArray(values, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(20.0));
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "count", (double)cJSON_GetArraySize(values));
    cJSON *nullable = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "optional", nullable);
    cJSON *pi_item = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(pi_item, 3.1415);
    cJSON_AddItemToObject(root, "pi", pi_item);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemViaPointer(root, values);
    double detached_len = (double)cJSON_GetArraySize(detached);
    cJSON_AddNumberToObject(root, "detached_len", detached_len);
    cJSON_bool detached_is_array = cJSON_IsArray(detached);
    cJSON_AddBoolToObject(root, "detached_is_array", detached_is_array);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}