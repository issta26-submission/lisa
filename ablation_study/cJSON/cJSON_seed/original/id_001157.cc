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
//<ID> 1157
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
    cJSON_AddItemToObject(root, "child", child);
    cJSON *name = cJSON_CreateString("initial");
    cJSON_AddItemToObject(child, "name", name);
    cJSON *num = cJSON_CreateNumber(123.0);
    cJSON_AddItemToObject(child, "value", num);
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddItemToArray(arr, cJSON_CreateString("one"));
    cJSON_AddItemToArray(arr, cJSON_CreateString("two"));

    // step 2: Configure
    char *new_name_ptr = cJSON_SetValuestring(name, "updated");
    cJSON_bool child_is_object = cJSON_IsObject(child);
    cJSON_bool child_is_invalid = cJSON_IsInvalid(child);
    cJSON_AddNumberToObject(child, "flags", (double)((int)child_is_object + (int)child_is_invalid));

    // step 3: Operate
    cJSON *detached = cJSON_DetachItemFromObject(root, "list");
    cJSON_AddItemToObject(child, "relist", detached);
    (void)new_name_ptr;

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