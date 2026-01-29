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
//<ID> 1565
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
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *item_a = cJSON_CreateString("first");
    cJSON *item_c = cJSON_CreateString("third");
    cJSON_AddItemToArray(arr, item_a);
    cJSON_AddItemToArray(arr, item_c);
    cJSON *item_b = cJSON_CreateString("second");
    cJSON_InsertItemInArray(arr, 1, item_b);

    // step 2: Configure
    cJSON_AddStringToObject(root, "info", "metadata");
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddNumberToObject(meta, "count", (double)cJSON_GetArraySize(arr));
    cJSON *detached_info = cJSON_DetachItemFromObject(root, "info");
    char *detached_val = cJSON_GetStringValue(detached_info);

    // step 3: Operate and Validate
    char *out = cJSON_PrintBuffered(root, 256, 1);
    size_t out_len = out ? strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = out_len ? out[0] : ' ';
    int size = cJSON_GetArraySize(arr);
    scratch[1] = (char)('0' + (size % 10));
    scratch[2] = detached_val ? detached_val[0] : ' ';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(detached_info);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}