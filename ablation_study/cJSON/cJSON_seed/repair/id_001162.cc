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
//<ID> 1162
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON *missing = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON_AddItemToObject(settings, "value", num);
    cJSON_AddItemToObject(settings, "missing", missing);

    // step 2: Configure
    char *snapshot1 = cJSON_PrintUnformatted(root);
    void *buffer = cJSON_malloc(64);
    memset(buffer, 0, 64);
    ((char *)buffer)[0] = snapshot1[0];
    ((char *)buffer)[1] = snapshot1[1];

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemFromObject(settings, "missing");
    cJSON_bool was_null = cJSON_IsNull(detached);
    cJSON_AddItemToObject(root, "orphan", detached);
    cJSON_bool is_obj = cJSON_IsObject(settings);
    double newval = cJSON_SetNumberHelper(num, 84.0);
    (void)was_null;
    (void)is_obj;
    (void)newval;

    // step 4: Finalize and Cleanup
    char *snapshot2 = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot1);
    cJSON_free(snapshot2);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}