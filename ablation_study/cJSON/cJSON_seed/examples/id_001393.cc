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
//<ID> 1393
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *flag_true = cJSON_CreateTrue();
    cJSON *initial_num = cJSON_CreateNumber(42.5);
    cJSON_AddItemToObject(child, "flag", flag_true);
    cJSON_AddItemToObject(child, "value", initial_num);
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    cJSON *replacement_num = cJSON_CreateNumber(100.0);
    cJSON *value_ptr = cJSON_GetObjectItem(child, "value");
    cJSON_ReplaceItemViaPointer(child, value_ptr, replacement_num);

    // step 3: Operate & Validate
    cJSON_bool is_true = cJSON_IsTrue(flag_true);
    double current = cJSON_GetNumberValue(replacement_num);
    double updated = current + (double)is_true;
    cJSON_SetNumberHelper(replacement_num, updated);
    int buf_len = 256;
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