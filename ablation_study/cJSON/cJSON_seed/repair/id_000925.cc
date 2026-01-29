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
//<ID> 925
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *settings = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "settings", settings);

    // step 2: Configure
    cJSON_AddTrueToObject(settings, "enabled");
    cJSON_AddStringToObject(root, "name", "node1");
    cJSON_AddNumberToObject(root, "id", 42.0);
    cJSON_AddItemToArray(items, cJSON_CreateString("first"));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(7.0));
    cJSON_AddItemToObject(root, "to_detach", cJSON_CreateString("temporary"));

    // step 3: Operate and Validate
    cJSON *got_enabled = cJSON_GetObjectItemCaseSensitive(settings, "enabled");
    cJSON_bool is_true = cJSON_IsTrue(got_enabled);
    cJSON_AddNumberToObject(root, "enabled_flag", (double)is_true);
    int count = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(root, "count", (double)count);
    cJSON *detached = cJSON_DetachItemFromObject(root, "to_detach");
    cJSON_AddItemToArray(items, detached);
    int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    cJSON_AddNumberToObject(root, "printed_flag", (double)printed);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}