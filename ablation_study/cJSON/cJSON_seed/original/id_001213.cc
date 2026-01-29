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
//<ID> 1213
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
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *null_item = cJSON_CreateNull();
    cJSON *label = cJSON_CreateString("original");

    // step 2: Configure
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_bool added_true = cJSON_AddItemToArray(arr, true_item);
    cJSON_bool added_null = cJSON_AddItemToArray(arr, null_item);
    cJSON *obj_null = cJSON_AddNullToObject(root, "explicit_null");
    char *new_val = cJSON_SetValuestring(label, "updated");
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    int arr_size = cJSON_GetArraySize(arr);
    int buf_len = 512;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_bool prealloc_ok = cJSON_PrintPreallocated(root, buf, buf_len, 1);
    const char *retrieved_label = cJSON_GetStringValue(cJSON_GetObjectItem(root, "label"));

    // step 4: Validate & Cleanup
    (void)added_true;
    (void)added_null;
    (void)obj_null;
    (void)new_val;
    (void)arr_size;
    (void)prealloc_ok;
    (void)retrieved_label;
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}