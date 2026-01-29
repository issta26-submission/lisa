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
//<ID> 222
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
    cJSON *b_false = cJSON_CreateFalse();
    cJSON *b_true = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, b_true);
    cJSON *num = cJSON_CreateNumber(123.0);
    cJSON_AddItemToArray(arr, num);
    cJSON *explicit_bool = cJSON_AddBoolToObject(root, "explicit_false", 0);

    // step 2: Configure
    cJSON_bool ref_added = cJSON_AddItemReferenceToArray(arr, b_false);
    int size_before = cJSON_GetArraySize(arr);
    cJSON *first_elem = cJSON_GetArrayItem(arr, 0);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemFromArray(arr, 2);
    cJSON_bool was_false = cJSON_IsFalse(detached);
    cJSON_AddItemToObject(root, "moved", detached);
    char *printed = cJSON_PrintUnformatted(root);
    void *tmp_buf = cJSON_malloc(64);
    memset(tmp_buf, 0, 64);
    int summary = (int)ref_added + size_before + (int)was_false + (first_elem ? first_elem->type : 0);
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}