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
//<ID> 1395
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(10.0);
    cJSON *bool_item = cJSON_CreateTrue();
    cJSON *str_item = cJSON_CreateString("initial");
    cJSON_AddItemToObject(root, "number", num_item);
    cJSON_AddItemToObject(root, "flag", bool_item);
    cJSON_AddItemToObject(root, "text", str_item);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_ReplaceItemViaPointer(root, str_item, replacement);
    int buf_len = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 1);

    // step 3: Operate & Validate
    cJSON_bool was_true = cJSON_IsTrue(bool_item);
    double current = cJSON_GetNumberValue(num_item);
    double new_val = current + (double)was_true;
    cJSON_SetNumberHelper(num_item, new_val);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_AddRawToObject(root, "snapshot", snapshot);
    cJSON_free((void *)snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free((void *)buffer);
    // API sequence test completed successfully
    return 66;
}