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
//<ID> 218
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *const words[] = { "apple", "banana", "cherry" };
    cJSON *str_arr = cJSON_CreateStringArray(words, 3);
    cJSON_AddItemToObject(root, "fruits", str_arr);
    cJSON *count_obj = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(root, "count", count_obj);

    // step 2: Configure
    double set_val = cJSON_SetNumberHelper(count_obj, 42.0);
    cJSON *dup_arr = cJSON_Duplicate(str_arr, 1);
    cJSON_AddItemToObject(root, "fruits_copy", dup_arr);

    // step 3: Operate and Validate
    cJSON *moved = cJSON_DetachItemViaPointer(root, dup_arr);
    cJSON *collector = cJSON_CreateArray();
    cJSON_AddItemToArray(collector, moved);
    cJSON_AddItemToObject(root, "collected", collector);
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    void *tmp_buf = cJSON_malloc(64);
    memset(tmp_buf, 0, 64);
    int size_original = cJSON_GetArraySize(str_arr);
    double number_value = cJSON_GetNumberValue(count_obj);
    int summary = (int)set_val + size_original + (int)number_value + root->type;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp_buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}