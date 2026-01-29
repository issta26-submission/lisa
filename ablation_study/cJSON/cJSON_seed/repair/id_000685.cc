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
//<ID> 685
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
    cJSON *num_item = cJSON_CreateNumber(3.14159);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(child, "val", num_item);
    cJSON_AddItemToObject(child, "empty", null_item);

    // step 2: Configure
    cJSON_AddItemToObject(root, "child", child);
    cJSON *direct_num = cJSON_AddNumberToObject(root, "direct", 1.0);
    cJSON_SetNumberHelper(direct_num, 2.71828);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemViaPointer(child, num_item);
    double detached_value = cJSON_GetNumberValue(detached);
    (void)detached_value;
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}