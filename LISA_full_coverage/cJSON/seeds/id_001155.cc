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
//<ID> 1155
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "greet", "hello");
    cJSON_AddItemToObject(root, "child", child);
    cJSON *arr = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("one");
    cJSON_AddItemToArray(arr, s1);
    cJSON *sref = cJSON_CreateStringReference("orig");
    cJSON_AddItemToArray(arr, sref);
    cJSON_AddItemToObject(root, "list", arr);

    // step 2: Configure
    cJSON *num = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(root, "pi", num);
    char *set_ret = cJSON_SetValuestring(s1, "uno");
    (void)set_ret;

    // step 3: Operate
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    cJSON *detached = cJSON_DetachItemFromObject(root, "child");
    cJSON_bool detached_is_obj = cJSON_IsObject(detached);
    cJSON_bool detached_is_invalid = cJSON_IsInvalid(detached);
    (void)root_is_obj;
    (void)detached_is_obj;
    (void)detached_is_invalid;
    cJSON_AddItemToObject(root, "restored", detached);
    const char *first_val = cJSON_GetStringValue(s1);
    cJSON_AddStringToObject(root, "first_value", first_val);

    // step 4: Validate & Cleanup
    char *flat = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);
    cJSON_free(flat);
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}