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
//<ID> 1373
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
    cJSON *greeting = cJSON_CreateString("hello");
    cJSON *nil_item = cJSON_CreateNull();

    // step 2: Configure
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(root, "greeting", greeting);
    cJSON_AddItemToObject(root, "nil", nil_item);
    cJSON *num_item = cJSON_AddNumberToObject(root, "value", 3.14);

    // step 3: Operate & Validate
    cJSON *retrieved_null = cJSON_GetObjectItem(root, "nil");
    cJSON_IsNull(retrieved_null);
    cJSON *retrieved_num = cJSON_GetObjectItem(root, "value");
    cJSON *replacement_num = cJSON_CreateNumber(42.0);
    cJSON_ReplaceItemViaPointer(root, retrieved_num, replacement_num);
    int buf_len = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    cJSON *snapshot = cJSON_CreateString(buffer);
    cJSON_AddItemToObject(root, "snapshot", snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(buffer);
    // API sequence test completed successfully
    return 66;
}