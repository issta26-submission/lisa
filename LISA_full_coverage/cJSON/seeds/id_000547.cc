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
//<ID> 547
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *colors[] = { "red", "green", "blue" };
    cJSON *arr = cJSON_CreateStringArray(colors, 3);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "colors", arr);
    cJSON *count_item = cJSON_AddNumberToObject(root, "count", 3.0);
    cJSON *magic_num = cJSON_CreateNumber(7.5);
    cJSON_AddItemToObject(root, "magic", magic_num);

    // step 2: Configure
    cJSON *dup_arr = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "colors_dup", dup_arr);
    double magic_val = cJSON_GetNumberValue(magic_num);
    double sum_val = 3.0 + magic_val;
    cJSON *sum = cJSON_CreateNumber(sum_val);
    cJSON_AddItemToObject(root, "sum", sum);

    // step 3: Operate
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    cJSON_bool arrays_equal = cJSON_Compare(arr, dup_arr, 1);
    cJSON_bool roots_equal = cJSON_Compare(root, dup_root, 1);
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t buf_len = 512;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);

    // step 4: Validate & Cleanup
    (void)arrays_equal;
    (void)roots_equal;
    (void)count_item;
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}