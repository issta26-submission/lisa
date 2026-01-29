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
//<ID> 1561
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
    cJSON *item0 = cJSON_CreateString("first");
    cJSON *item2 = cJSON_CreateString("third");
    cJSON_AddItemToArray(arr, item0);
    cJSON_AddItemToArray(arr, item2);
    cJSON *item1 = cJSON_CreateString("second");
    cJSON_InsertItemInArray(arr, 1, item1);
    cJSON_AddItemToObject(root, "items", arr);

    // step 2: Configure
    int size = cJSON_GetArraySize(arr);
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    char *second_str = (char *)cJSON_GetStringValue(second);
    cJSON *count = cJSON_CreateNumber((double)size);
    cJSON_AddItemToObject(root, "count", count);

    // step 3: Operate and Validate
    cJSON *detached_arr = cJSON_DetachItemFromObject(root, "items");
    char *out = cJSON_PrintBuffered(detached_arr, 128, 1);
    size_t out_len = out ? strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 8);
    memset(scratch, 0, out_len + 8);
    scratch[0] = out_len ? out[0] : '\0';
    scratch[1] = (char)('0' + (int)size);
    scratch[2] = second_str ? second_str[0] : '\0';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(detached_arr);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}