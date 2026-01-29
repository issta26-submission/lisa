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
//<ID> 1399
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *item_str = cJSON_CreateString("initial_value");
    cJSON_AddItemToObject(root, "name", item_str);
    cJSON *num_item = cJSON_CreateNumber(10.0);
    cJSON_AddItemToObject(root, "count", num_item);
    cJSON *flag_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", flag_item);

    // step 2: Configure
    cJSON *replacement_str = cJSON_CreateString("replaced_value");
    cJSON *old_ptr = cJSON_GetObjectItem(root, "name");
    cJSON_ReplaceItemViaPointer(root, old_ptr, replacement_str);
    int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 1);

    // step 3: Operate & Validate
    cJSON *flag_ptr = cJSON_GetObjectItem(root, "enabled");
    cJSON_bool is_true = cJSON_IsTrue(flag_ptr);
    double current = cJSON_GetNumberValue(num_item);
    double updated = current + (double)is_true;
    cJSON_SetNumberHelper(num_item, updated);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_AddRawToObject(root, "snapshot", snapshot);
    cJSON_free((void *)snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free((void *)buffer);
    // API sequence test completed successfully
    return 66;
}