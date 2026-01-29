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
//<ID> 1151
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
    cJSON *s = cJSON_CreateString("initial");
    cJSON *n = cJSON_CreateNumber(123.0);
    cJSON_AddItemToObject(child, "s", s);
    cJSON_AddItemToObject(child, "n", n);
    cJSON_AddItemToObject(root, "child", child);
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON *str_item = cJSON_CreateString("to_change");
    cJSON_AddItemToArray(arr, str_item);

    // step 2: Configure
    char *set_ret = cJSON_SetValuestring(str_item, "changed");
    cJSON_bool is_root_obj = cJSON_IsObject(root);
    cJSON_bool is_child_obj = cJSON_IsObject(child);

    // step 3: Operate
    cJSON_bool is_invalid_before = cJSON_IsInvalid(n);
    cJSON *detached = cJSON_DetachItemFromObject(child, "n");
    cJSON_AddItemToObject(root, "detached_number", detached);
    cJSON *got_s = cJSON_GetObjectItem(child, "s");
    char *set_ret2 = cJSON_SetValuestring(got_s, "inner_changed");

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
    (void)set_ret;
    (void)set_ret2;
    (void)is_root_obj;
    (void)is_child_obj;
    (void)is_invalid_before;
    return 66;
}