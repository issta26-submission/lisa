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
//<ID> 929
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

    // step 2: Configure
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *active = cJSON_AddTrueToObject(root, "active");
    cJSON *id = cJSON_CreateNumber(123.0);
    cJSON_AddItemToObject(root, "id", id);
    cJSON *name = cJSON_CreateString("device_alpha");
    cJSON_AddItemToObject(meta, "name", name);
    cJSON_AddStringToObject(root, "version", "1.0");

    // step 3: Operate and Validate
    cJSON_AddItemToArray(items, cJSON_CreateString("one"));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(2.0));
    cJSON *queried_active = cJSON_GetObjectItemCaseSensitive(root, "active");
    cJSON_bool active_flag = cJSON_IsTrue(queried_active);
    cJSON_AddNumberToObject(root, "active_flag", (double)active_flag);
    int buflen = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    (void)printed;
    cJSON *dup = cJSON_Duplicate(root, 1);
    char *snapshot = cJSON_PrintUnformatted(dup);
    cJSON_free(snapshot);
    cJSON_Delete(dup);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}