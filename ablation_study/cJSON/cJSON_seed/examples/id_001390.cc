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
//<ID> 1390
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *flag_item = cJSON_CreateTrue();
    cJSON *count_item = cJSON_CreateNumber(10.0);
    cJSON *label_item = cJSON_CreateString("initial_label");
    cJSON_AddItemToObject(root, "flag", flag_item);
    cJSON_AddItemToObject(root, "count", count_item);
    cJSON_AddItemToObject(root, "label", label_item);

    // step 2: Configure
    cJSON *new_label = cJSON_CreateString("updated_label");
    cJSON *old_label_ptr = cJSON_GetObjectItem(root, "label");
    cJSON_ReplaceItemViaPointer(root, old_label_ptr, new_label);
    int buf_len = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);

    // step 3: Operate & Validate
    cJSON_bool flag_true = cJSON_IsTrue(flag_item);
    double current = cJSON_GetNumberValue(count_item);
    double updated = current + (double)flag_true;
    cJSON_SetNumberHelper(count_item, updated);
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