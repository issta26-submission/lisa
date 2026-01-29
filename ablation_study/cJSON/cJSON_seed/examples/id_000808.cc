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
//<ID> 808
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

    // step 2: Configure
    cJSON *active_item = cJSON_AddTrueToObject(root, "active");
    cJSON *version_ref = cJSON_CreateStringReference(version_str);
    cJSON_AddItemToObject(root, "version", version_ref);

    // step 3: Operate & Validate
    char *snapshot = cJSON_Print(root);
    size_t snapshot_len = strlen(snapshot);
    cJSON_AddNumberToObject(root, "snapshot_len", (double)snapshot_len);
    cJSON *root_dup = cJSON_Duplicate(root, 1);
    cJSON_bool are_equal = cJSON_Compare(root, root_dup, 1);
    cJSON_AddBoolToObject(root, "is_equal", are_equal);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root_dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}