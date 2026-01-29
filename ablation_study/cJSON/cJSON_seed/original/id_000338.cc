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
//<ID> 338
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *placeholder = cJSON_CreateNull();
    cJSON_AddItemToObjectCS(root, "replace_me", placeholder);
    int numbers[] = {10, 20, 30};
    cJSON *int_array = cJSON_CreateIntArray(numbers, 3);

    // step 2: Configure
    cJSON_ReplaceItemViaPointer(root, placeholder, int_array);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "meta", meta);

    // step 3: Operate and Validate
    cJSON *retrieved = cJSON_GetObjectItem(root, "replace_me");
    int arr_size = cJSON_GetArraySize(retrieved);
    cJSON_AddItemToObjectCS(meta, "len", cJSON_CreateNumber((double)arr_size));
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON_bool prealloc_ok = cJSON_PrintPreallocated(root, buffer, 256, 0);
    cJSON_AddItemToObjectCS(meta, "printed_prealloc", cJSON_CreateBool(prealloc_ok));

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}