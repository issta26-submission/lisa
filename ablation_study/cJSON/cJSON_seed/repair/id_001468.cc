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
//<ID> 1468
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"meta\":{\"id\":1},\"items\":[10,20,30]}";
    cJSON *root = cJSON_Parse(json_text);
    cJSON *items = cJSON_GetObjectItem(root, "items");
    int orig_size = cJSON_GetArraySize(items);

    // step 2: Configure
    cJSON *dup_items = cJSON_Duplicate(items, 1);
    cJSON *new_root = cJSON_CreateObject();
    cJSON_AddItemToObject(new_root, "copied_items", dup_items);

    // step 3: Operate and Validate
    cJSON *copied = cJSON_GetObjectItem(new_root, "copied_items");
    int dup_size = cJSON_GetArraySize(copied);
    char *out = cJSON_PrintUnformatted(new_root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 8);
    memset(scratch, 0, out_len + 8);
    scratch[0] = out_len ? out[0] : 'x';
    scratch[1] = (char)('0' + (orig_size % 10));
    scratch[2] = (char)('0' + (dup_size % 10));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(new_root);
    // API sequence test completed successfully
    return 66;
}