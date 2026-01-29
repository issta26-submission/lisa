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
//<ID> 1250
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
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(child, "arr", arr);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(1.5);
    cJSON_AddItemToArray(arr, n1);
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(arr, n2);
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "name", name);

    // step 3: Operate
    cJSON *got_child = cJSON_GetObjectItem(root, "child");
    cJSON_bool is_invalid = cJSON_IsInvalid(got_child);
    (void)is_invalid;
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buf, buf_len, 1);
    (void)printed_ok;
    cJSON *detached = cJSON_DetachItemViaPointer(root, got_child);
    cJSON_Delete(detached);

    // step 4: Validate & Cleanup
    cJSON *got_name = cJSON_GetObjectItem(root, "name");
    const char *name_str = cJSON_GetStringValue(got_name);
    (void)name_str;
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}