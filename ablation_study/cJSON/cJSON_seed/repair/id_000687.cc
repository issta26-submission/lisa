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
//<ID> 687
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *val_item = cJSON_AddNumberToObject(config, "value", 3.14159);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(config, "maybe_null", null_item);

    // step 2: Configure
    double newval = cJSON_SetNumberHelper(val_item, 6.28318);
    cJSON *detached = cJSON_DetachItemViaPointer(config, null_item);
    cJSON_AddItemToObject(root, "orphan", detached);

    // step 3: Operate and Validate
    double verified = cJSON_GetNumberValue(val_item);
    cJSON *computed_item = cJSON_AddNumberToObject(root, "computed", verified * 2.0);
    char *buffer = (char *)cJSON_malloc(512);
    memset(buffer, 0, 512);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, 512, 0);
    (void)printed_ok;
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}