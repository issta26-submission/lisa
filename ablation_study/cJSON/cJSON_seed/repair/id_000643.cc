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
//<ID> 643
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "lang", "en");
    cJSON_AddStringToObject(root, "greeting", "hello");
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag);

    // step 2: Configure
    char *greeting = cJSON_GetStringValue(cJSON_GetObjectItem(root, "greeting"));
    cJSON_AddStringToObject(root, "greeting_copy", greeting);
    cJSON_bool flag_is_false = cJSON_IsFalse(cJSON_GetObjectItem(root, "flag"));
    cJSON_AddItemToObject(meta, "flag_state", cJSON_CreateBool(flag_is_false));

    // step 3: Operate and Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_AddStringToObject(root, "snapshot", snapshot);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}