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
//<ID> 964
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *message = cJSON_CreateString("initial");

    // step 2: Configure
    char *set_ret = cJSON_SetValuestring(message, "hello world");
    cJSON_AddItemToObject(root, "message", message);
    cJSON *copy = cJSON_CreateString(set_ret);
    cJSON_AddItemToObject(root, "message_copy", copy);
    cJSON_AddFalseToObject(root, "active");

    // step 3: Operate & Validate
    cJSON *got_copy = cJSON_GetObjectItem(root, "message_copy");
    char *extracted = cJSON_GetStringValue(got_copy);
    cJSON_SetValuestring(copy, extracted);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}