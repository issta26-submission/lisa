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
//<ID> 1370
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *greet = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greeting", greet);
    cJSON *ans = cJSON_AddNumberToObject(root, "answer", 42.0);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("world");
    cJSON *old_greet = cJSON_GetObjectItem(root, "greeting");
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(root, old_greet, replacement);

    // step 3: Operate & Validate
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "empty", null_item);
    cJSON_bool is_null = cJSON_IsNull(null_item);
    cJSON *flagnum = cJSON_AddNumberToObject(root, "null_flag", (double)is_null);
    int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 1);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(buffer);
    // API sequence test completed successfully
    return 66;
}