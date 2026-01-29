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
//<ID> 1146
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *words[] = { "alpha", "beta", "gamma" };
    cJSON *str_arr = cJSON_CreateStringArray(words, 3);
    cJSON_AddItemToObject(root, "strs", str_arr);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(child, "child_key", cJSON_CreateString("child_val"));
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    cJSON *orig_child = cJSON_GetObjectItem(root, "child");
    cJSON *replacement = cJSON_CreateObject();
    cJSON_AddItemToObject(replacement, "replaced_key", cJSON_CreateString("newval"));
    cJSON_ReplaceItemViaPointer(root, orig_child, replacement);
    cJSON *new_child = cJSON_GetObjectItem(root, "child");
    cJSON *child_ref = cJSON_CreateObjectReference(new_child);
    cJSON_AddItemToObject(root, "child_ref", child_ref);

    // step 3: Operate
    int arr_size = cJSON_GetArraySize(str_arr);
    cJSON_AddNumberToObject(root, "arr_size", (double)arr_size);
    cJSON_DeleteItemFromObject(root, "strs");

    // step 4: Validate & Cleanup
    char *flat = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);
    cJSON_free(flat);
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}