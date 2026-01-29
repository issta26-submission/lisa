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
//<ID> 689
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
    cJSON *num_item = cJSON_AddNumberToObject(child, "initial", 3.14159);
    cJSON *maybe = cJSON_CreateNull();
    cJSON_AddItemToObject(child, "maybe", maybe);

    // step 2: Configure
    double updated_value = cJSON_SetNumberHelper(num_item, 6.28318);
    cJSON *detached = cJSON_DetachItemViaPointer(child, maybe);

    // step 3: Operate and Validate
    cJSON_AddItemToObject(root, "maybe", detached);
    cJSON_AddNumberToObject(root, "updated_copy", updated_value);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}