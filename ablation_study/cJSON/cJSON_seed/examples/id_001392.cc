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
//<ID> 1392
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *old_str = cJSON_CreateString("old_value");
    cJSON_AddItemToObject(root, "old", old_str);
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "counter", num_item);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", true_item);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("new_value");
    cJSON *old_item_ptr = cJSON_GetObjectItem(root, "old");
    cJSON_ReplaceItemViaPointer(root, old_item_ptr, replacement);

    // step 3: Operate & Validate
    cJSON_bool flag_set = cJSON_IsTrue(true_item);
    double current = cJSON_GetNumberValue(num_item);
    double updated = current + (flag_set ? 1.0 : 0.0);
    cJSON_SetNumberHelper(num_item, updated);
    int buf_len = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_AddRawToObject(root, "snapshot", snapshot);
    cJSON_free((void *)snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free((void *)buffer);
    // API sequence test completed successfully
    return 66;
}