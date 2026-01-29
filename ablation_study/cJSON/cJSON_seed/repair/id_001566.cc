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
//<ID> 1566
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
    cJSON *first = cJSON_CreateString("one");
    cJSON_AddItemToArray(arr, first);
    cJSON *second = cJSON_CreateString("two");
    cJSON_InsertItemInArray(arr, 1, second);
    cJSON *sub = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "sub", sub);
    cJSON_AddStringToObject(sub, "name", "example");

    // step 2: Configure
    int size_before = cJSON_GetArraySize(arr);
    cJSON *detached_sub = cJSON_DetachItemFromObject(root, "sub");
    cJSON *name_item = cJSON_GetObjectItem(detached_sub, "name");
    const char *name_str = cJSON_GetStringValue(name_item);
    cJSON *detached_name_copy = cJSON_CreateString(name_str);
    cJSON_AddItemToObject(root, "detached_name", detached_name_copy);

    // step 3: Operate and Validate
    int size_after = cJSON_GetArraySize(arr);
    cJSON *count_num = cJSON_CreateNumber((double)size_after);
    cJSON_AddItemToObject(root, "count", count_num);
    char *out = cJSON_PrintBuffered(root, 128, 1);
    size_t out_len = out ? strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = out_len ? out[0] : ' ';
    scratch[1] = (char)('0' + (int)size_before);
    scratch[2] = (char)('0' + (int)size_after);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(detached_sub);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}