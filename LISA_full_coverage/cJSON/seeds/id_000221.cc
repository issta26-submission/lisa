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
//<ID> 221
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
    cJSON *ref_item = cJSON_CreateString("referenced");
    cJSON_AddItemReferenceToArray(arr, ref_item);

    // step 2: Configure
    cJSON *detached = cJSON_DetachItemFromArray(arr, 0);
    cJSON_AddItemToObject(root, "moved", detached);
    cJSON *bool_item = cJSON_AddBoolToObject(root, "is_false", 0);
    cJSON_bool is_false = cJSON_IsFalse(bool_item);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    int arr_size = cJSON_GetArraySize(arr);
    const char *str_val = cJSON_GetStringValue(detached);
    double num_check = cJSON_GetNumberValue(ref_item);
    void *tmp_buf = cJSON_malloc(64);
    memset(tmp_buf, 0, 64);
    (void)is_false;
    (void)arr_size;
    (void)str_val;
    (void)num_check;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp_buf);
    cJSON_Delete(root);

    return 66;
    // API sequence test completed successfully
}