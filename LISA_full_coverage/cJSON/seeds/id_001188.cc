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
//<ID> 1188
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "my_array", arr);
    cJSON *orig = cJSON_CreateString("old_value");
    cJSON_AddItemToObject(root, "to_replace", orig);
    cJSON *ref_target = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "ref_target", ref_target);

    // step 2: Configure
    cJSON_AddItemReferenceToArray(arr, ref_target);
    cJSON_bool arr_is_array = cJSON_IsArray(arr);
    cJSON_AddBoolToObject(root, "array_is_array", arr_is_array);
    cJSON *null_slot = cJSON_AddNullToObject(root, "maybe_null");

    // step 3: Operate
    cJSON *replacement = cJSON_CreateString("new_value");
    cJSON_ReplaceItemViaPointer(root, orig, replacement);
    char *flat = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 0);
    cJSON_free(buf);
    cJSON_free(flat);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}