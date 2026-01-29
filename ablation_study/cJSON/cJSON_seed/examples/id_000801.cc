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
//<ID> 801
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *ver = cJSON_Version();
    cJSON *ver_ref = cJSON_CreateStringReference(ver);

    // step 2: Configure
    cJSON_AddItemToObject(root, "version", ver_ref);
    cJSON *active = cJSON_AddTrueToObject(root, "active");

    // step 3: Operate & Validate
    char *snapshot = cJSON_Print(root);
    cJSON *version_item = cJSON_GetObjectItem(root, "version");
    char *extracted_version = cJSON_GetStringValue(version_item);
    cJSON *retrieved_active = cJSON_GetObjectItem(root, "active");
    cJSON_bool is_true = cJSON_IsTrue(retrieved_active);
    (void)extracted_version;
    (void)is_true;
    (void)active;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}