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
//<ID> 1001
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
    cJSON_AddStringToObject(root, "greeting", "hello");
    cJSON_AddStringToObject(root, "target", "world");

    // step 3: Operate and Validate
    cJSON *greeting = cJSON_GetObjectItemCaseSensitive(root, "greeting");
    char *updated_value = cJSON_SetValuestring(greeting, "hi");
    cJSON *target = cJSON_GetObjectItemCaseSensitive(root, "target");
    cJSON_AddStringToObject(root, "greeting_copy", updated_value);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}