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
//<ID> 920
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *flags = cJSON_AddArrayToObject(root, "flags");
    cJSON *settings = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "settings", settings);

    // step 2: Configure
    cJSON *flag_true = cJSON_CreateTrue();
    cJSON_AddItemToArray(flags, flag_true);
    cJSON_AddTrueToObject(settings, "enabled");
    cJSON_AddStringToObject(root, "name", "node42");
    cJSON_AddNumberToObject(root, "count", 0.0);

    // step 3: Operate and Validate
    cJSON *enabled_item = cJSON_GetObjectItemCaseSensitive(settings, "enabled");
    cJSON_bool enabled_is_true = cJSON_IsTrue(enabled_item);
    (void)enabled_is_true;
    cJSON *count_item = cJSON_GetObjectItemCaseSensitive(root, "count");
    int flags_size = cJSON_GetArraySize(flags);
    double current_count = cJSON_GetNumberValue(count_item);
    (void)cJSON_SetNumberHelper(count_item, current_count + (double)flags_size);
    int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    (void)printed;
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}