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
//<ID> 226
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
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON *f1 = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, f1);
    cJSON *ref_arr = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("hello");
    cJSON_bool ref_added = cJSON_AddItemReferenceToArray(ref_arr, s1);
    cJSON_AddItemToObject(root, "refs", ref_arr);

    // step 2: Configure
    cJSON *detached = cJSON_DetachItemFromArray(ref_arr, 0);
    cJSON_AddItemToObject(root, "detached", detached);
    cJSON *enabled_ptr = cJSON_AddBoolToObject(root, "enabled", 0);

    // step 3: Operate and Validate
    int arr_size = cJSON_GetArraySize(arr);
    cJSON *enabled_get = cJSON_GetObjectItem(root, "enabled");
    cJSON_bool is_false = cJSON_IsFalse(enabled_get);
    double number_val = cJSON_GetNumberValue(n1);
    char *printed = cJSON_Print(root);
    void *tmp_buf = cJSON_malloc(64);
    memset(tmp_buf, 0, 64);
    int summary = arr_size + (int)number_val + (is_false ? 1 : 0) + (printed ? (int)printed[0] : 0);
    (void)summary;
    (void)ref_added;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}