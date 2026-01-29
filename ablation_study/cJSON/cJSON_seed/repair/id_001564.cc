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
//<ID> 1564
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
    cJSON *item_a = cJSON_CreateString("alpha");
    cJSON_InsertItemInArray(arr, 0, item_a);
    cJSON *item_b = cJSON_CreateString("beta");
    cJSON_InsertItemInArray(arr, 1, item_b);
    cJSON *greet = cJSON_CreateString("hello world");
    cJSON_AddItemToObject(root, "greeting", greet);

    // step 2: Configure
    int count = cJSON_GetArraySize(arr);
    cJSON *info = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "info", info);
    cJSON *count_num = cJSON_CreateNumber((double)count);
    cJSON_AddItemToObject(info, "count", count_num);

    // step 3: Operate and Validate
    cJSON *detached_greet = cJSON_DetachItemFromObject(root, "greeting");
    char *greet_str = cJSON_GetStringValue(detached_greet);
    int prebuffer = 64 + count * 16;
    char *out = cJSON_PrintBuffered(root, prebuffer, 1);
    size_t out_len = out ? strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 32);
    memset(scratch, 0, out_len + 32);
    scratch[0] = greet_str ? greet_str[0] : ' ';
    scratch[1] = out_len ? out[0] : ' ';
    scratch[2] = (char)('0' + (count % 10));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(detached_greet);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}