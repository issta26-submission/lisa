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
//<ID> 1251
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
    cJSON_AddItemToObject(root, "arr", arr);

    // step 2: Configure
    cJSON_AddNumberToObject(child, "number", 123.0);
    cJSON_AddStringToObject(child, "text", "hello");
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2.0));

    // step 3: Operate
    cJSON *got_child = cJSON_GetObjectItem(root, "child");
    cJSON *num_item = cJSON_GetObjectItem(got_child, "number");
    cJSON_bool is_invalid = cJSON_IsInvalid(num_item);
    cJSON *text_item = cJSON_GetObjectItem(got_child, "text");
    cJSON *detached = cJSON_DetachItemViaPointer(got_child, text_item);
    cJSON_AddItemToObject(root, "detached_text", detached);
    int buf_len = 512;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);

    // step 4: Validate & Cleanup
    cJSON *detached_lookup = cJSON_GetObjectItem(root, "detached_text");
    char *text_value = cJSON_GetStringValue(detached_lookup);
    (void)is_invalid;
    (void)text_value;
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}