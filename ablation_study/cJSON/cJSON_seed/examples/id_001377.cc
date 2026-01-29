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
//<ID> 1377
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
    cJSON *old_field = cJSON_CreateString("old_value");
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(child, "field", old_field);
    cJSON_AddItemToObject(root, "child", child);
    cJSON *num_ptr = cJSON_AddNumberToObject(root, "count", 42.0);
    cJSON_AddItemToObject(root, "nullable", null_item);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("new_value");
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(child, old_field, replacement);
    cJSON *extra_str = cJSON_CreateString("note");
    cJSON_AddItemToObject(root, "note", extra_str);

    // step 3: Operate & Validate
    cJSON_bool is_null = cJSON_IsNull(null_item);
    int buf_len = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    cJSON *raw = cJSON_CreateRaw(buffer);
    cJSON_AddItemToObject(root, "snapshot", raw);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(buffer);
    // API sequence test completed successfully
    return 66;
}