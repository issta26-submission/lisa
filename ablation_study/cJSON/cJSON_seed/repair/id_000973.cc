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
//<ID> 973
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *temp = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "tempCS", temp);
    cJSON *to_detach = cJSON_CreateString("will_be_detached");
    cJSON_AddItemToObject(root, "detach_me", to_detach);

    // step 2: Configure
    cJSON_AddNumberToObject(config, "timeout", 30.0);
    cJSON *sub = cJSON_AddObjectToObject(config, "sub");
    cJSON_AddBoolToObject(sub, "enabled", 1);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemFromObject(root, "detach_me");
    cJSON_AddItemToObjectCS(temp, "reattached", detached);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    (void)printed;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}