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
//<ID> 733
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
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "value", num);
    cJSON *value_ref = cJSON_GetObjectItem(root, "value");

    // step 2: Configure
    cJSON *num_dup = cJSON_Duplicate(value_ref, 1);
    cJSON *replacement = cJSON_CreateNumber(100.5);

    // step 3: Operate and Validate
    cJSON_ReplaceItemViaPointer(root, value_ref, replacement);
    cJSON_AddItemToObject(meta, "orig_copy", num_dup);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}