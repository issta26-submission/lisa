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
//<ID> 1000
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");

    // step 2: Configure
    cJSON_AddStringToObject(meta, "greeting", "hello");
    cJSON_AddStringToObject(root, "status", "active");

    // step 3: Operate and Validate
    cJSON *greet = cJSON_GetObjectItemCaseSensitive(meta, "greeting");
    cJSON_SetValuestring(greet, "world");
    char *greet_val = cJSON_GetStringValue(greet);
    cJSON_AddStringToObject(root, "greeting_copy", greet_val);
    char *serialized = cJSON_PrintUnformatted(root);
    cJSON_free(serialized);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}