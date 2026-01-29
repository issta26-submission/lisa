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
//<ID> 809
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *version_str = cJSON_Version();
    cJSON *version_ref = cJSON_CreateStringReference(version_str);
    cJSON_AddTrueToObject(root, "initialized");
    cJSON_AddItemToObject(root, "version", version_ref);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *owner_ref = cJSON_CreateStringReference("fuzzer-owner");
    cJSON_AddItemToObject(meta, "owner", owner_ref);

    // step 3: Operate & Validate
    char *snapshot = cJSON_Print(root);
    cJSON *version_item = cJSON_GetObjectItem(root, "version");
    const char *extracted_version = cJSON_GetStringValue(version_item);
    cJSON *extracted_ref = cJSON_CreateStringReference(extracted_version);
    cJSON_AddItemToObject(root, "extracted_version_ref", extracted_ref);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}