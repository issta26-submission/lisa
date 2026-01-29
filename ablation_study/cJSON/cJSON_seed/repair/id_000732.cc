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
//<ID> 732
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
    cJSON *old_num = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(meta, "value", old_num);
    cJSON *value_ptr = cJSON_GetObjectItem(meta, "value");

    // step 2: Configure
    cJSON *replacement = cJSON_CreateNumber(100.0);
    cJSON *dup_replacement = cJSON_Duplicate(replacement, 0);
    cJSON_ReplaceItemViaPointer(meta, value_ptr, dup_replacement);

    // step 3: Operate and Validate
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    cJSON *dup_meta = cJSON_GetObjectItem(dup_root, "meta");
    cJSON *dup_value = cJSON_GetObjectItem(dup_meta, "value");
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    cJSON_Delete(replacement);

    // API sequence test completed successfully
    return 66;
}