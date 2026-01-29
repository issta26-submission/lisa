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
//<ID> 46
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *nested = cJSON_AddObjectToObject(root, "nested");
    cJSON *key_item = cJSON_CreateString("initial");
    cJSON_AddItemToObject(nested, "key", key_item);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag);

    // step 2: Configure
    cJSON *nested_ref = cJSON_GetObjectItem(root, "nested");
    cJSON *retrieved_key = cJSON_GetObjectItem(nested_ref, "key");
    cJSON_SetValuestring(retrieved_key, "updated");
    cJSON *copied = cJSON_CreateString(cJSON_GetStringValue(retrieved_key));
    cJSON_AddItemToObject(root, "copied", copied);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}