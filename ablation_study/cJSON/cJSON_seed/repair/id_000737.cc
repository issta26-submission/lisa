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
//<ID> 737
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
    cJSON *version = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObject(meta, "version", version);

    // step 2: Configure
    cJSON *meta_copy = cJSON_Duplicate(meta, 1);
    cJSON_AddItemToObject(root, "meta_copy", meta_copy);

    // step 3: Operate and Validate
    cJSON *meta_ref = cJSON_GetObjectItem(root, "meta");
    cJSON *version_ref = cJSON_GetObjectItem(meta_ref, "version");
    cJSON *new_version = cJSON_CreateNumber(2.0);
    cJSON_ReplaceItemViaPointer(meta_ref, version_ref, new_version);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}