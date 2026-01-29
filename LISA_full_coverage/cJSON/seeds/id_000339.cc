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
//<ID> 339
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
    int nums1[] = {1, 2, 3};
    cJSON *int_arr = cJSON_CreateIntArray(nums1, 3);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObjectCS(child, "ints", int_arr);
    cJSON_AddItemToObjectCS(child, "optional", null_item);
    cJSON_AddItemToObjectCS(root, "child", child);

    // step 2: Configure
    int repl_nums[] = {9, 9};
    cJSON *repl_arr = cJSON_CreateIntArray(repl_nums, 2);
    cJSON_ReplaceItemViaPointer(child, null_item, repl_arr);

    // step 3: Operate
    cJSON *meta = cJSON_CreateObject();
    int orig_count = cJSON_GetArraySize(int_arr);
    int repl_count = cJSON_GetArraySize(repl_arr);
    cJSON *n_orig = cJSON_CreateNumber((double)orig_count);
    cJSON *n_repl = cJSON_CreateNumber((double)repl_count);
    cJSON_AddItemToObjectCS(meta, "orig_count", n_orig);
    cJSON_AddItemToObjectCS(meta, "repl_count", n_repl);
    cJSON *first = cJSON_GetArrayItem(int_arr, 0);
    cJSON *second = cJSON_GetArrayItem(int_arr, 1);
    double sum_first_two = cJSON_GetNumberValue(first) + cJSON_GetNumberValue(second);
    cJSON *sum_node = cJSON_CreateNumber(sum_first_two);
    cJSON_AddItemToObjectCS(meta, "sum_first_two", sum_node);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON_bool prealloc_ok = cJSON_PrintPreallocated(root, buffer, 256, 0);
    cJSON *prealloc_flag = cJSON_CreateBool(prealloc_ok);
    cJSON_AddItemToObjectCS(meta, "prealloc_ok", prealloc_flag);
    cJSON_AddItemToObjectCS(root, "meta", meta);

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}