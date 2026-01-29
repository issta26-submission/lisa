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
//<ID> 1136
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
    cJSON *s_alpha = cJSON_CreateString("alpha");
    cJSON *t_true = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, s_alpha);
    cJSON_AddItemToArray(arr, t_true);
    cJSON_AddItemToObject(root, "items", arr);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddTrueToObject(meta, "enabled");
    cJSON *name_item = cJSON_CreateString("original");
    cJSON_AddItemToObject(meta, "name", name_item);
    char *updated_ptr = cJSON_SetValuestring(name_item, "updated");

    // step 3: Operate and Validate
    int arr_size = cJSON_GetArraySize(arr);
    cJSON *count_item = cJSON_CreateNumber((double)arr_size);
    cJSON_AddItemToObject(root, "count", count_item);
    char *out = cJSON_PrintUnformatted(root);
    char *scratch = (char *)cJSON_malloc(64);
    memset(scratch, 0, 64);
    scratch[0] = out[0];
    scratch[1] = out[1];
    (void)updated_ptr;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}