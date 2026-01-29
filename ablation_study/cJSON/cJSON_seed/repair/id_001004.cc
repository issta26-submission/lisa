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
//<ID> 1004
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
    cJSON_AddStringToObject(root, "name", "original");
    cJSON_AddStringToObject(meta, "version", "1.0");
    cJSON_AddStringToObject(meta, "status", "active");

    // step 3: Operate and Validate
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(root, "name");
    char *updated_value = cJSON_SetValuestring(name_item, "modified");
    cJSON_AddStringToObject(root, "name_copy", updated_value);
    cJSON *snapshot = cJSON_Duplicate(root, 1);
    char *printed_root = cJSON_Print(root);
    cJSON_free(printed_root);
    char *printed_snapshot = cJSON_PrintUnformatted(snapshot);
    cJSON_free(printed_snapshot);

    // step 4: Cleanup
    cJSON_Delete(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}