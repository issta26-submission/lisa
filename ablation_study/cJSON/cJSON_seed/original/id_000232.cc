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
//<ID> 232
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
    cJSON *count = cJSON_AddNumberToObject(root, "count", 10.0);
    cJSON *bfalse = cJSON_CreateFalse();
    cJSON *str = cJSON_CreateString("example");
    cJSON *num_in_arr = cJSON_CreateNumber(3.5);
    cJSON_AddItemToArray(arr, bfalse);
    cJSON_AddItemToArray(arr, str);
    cJSON_AddItemToArray(arr, num_in_arr);

    // step 2: Configure
    cJSON *extra = cJSON_AddNumberToObject(root, "extra", 2.25);
    cJSON_bool was_bool = cJSON_IsBool(bfalse);
    int size_before = cJSON_GetArraySize(arr);

    // step 3: Operate and Validate
    char *printed = cJSON_Print(root);
    cJSON_DeleteItemFromArray(arr, 1);
    int size_after = cJSON_GetArraySize(arr);
    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_IsBool(first_item) ? 0.0 : cJSON_GetNumberValue(first_item);
    int summary = size_before + size_after + (int)cJSON_GetNumberValue(extra) + (int)first_val + (int)was_bool;
    void *tmp_buf = cJSON_malloc(128);
    memset(tmp_buf, 0, 128);
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}