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
//<ID> 1376
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
    cJSON_AddItemToObject(root, "child", child);
    cJSON *num_item = cJSON_AddNumberToObject(child, "value", 42.0);
    cJSON *label = cJSON_CreateString("initial_label");
    cJSON_AddItemToObject(root, "label", label);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("replaced_value");
    cJSON_ReplaceItemViaPointer(child, num_item, replacement);

    // step 3: Operate & Validate
    cJSON *retrieved = cJSON_GetObjectItem(child, "value");
    cJSON_bool was_null = cJSON_IsNull(retrieved);
    cJSON_AddNumberToObject(root, "was_null_flag", (double)was_null);
    int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    cJSON *printed_copy = cJSON_CreateString(buffer);
    cJSON_AddItemToObject(root, "printed", printed_copy);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(buffer);
    // API sequence test completed successfully
    return 66;
}