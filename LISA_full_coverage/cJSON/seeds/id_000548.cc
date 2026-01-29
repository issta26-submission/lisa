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
//<ID> 548
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *names[] = { "alpha", "beta", "gamma" };
    cJSON *arr = cJSON_CreateStringArray(names, 3);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "names", arr);
    cJSON *count_num = cJSON_CreateNumber(3.0);
    cJSON_AddItemToObject(root, "count", count_num);

    // step 2: Configure
    cJSON *dup_arr = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "names_copy", dup_arr);
    double sum_val = 3.0 + 5.0;
    cJSON_AddNumberToObject(root, "meta_sum", sum_val);

    // step 3: Operate
    cJSON_bool equal = cJSON_Compare(arr, dup_arr, 1);
    double equal_num = equal ? 1.0 : 0.0;
    cJSON_AddNumberToObject(root, "compare_result", equal_num);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON *first_dup = cJSON_Duplicate(first, 1);
    cJSON_AddItemToObject(root, "first_copy", first_dup);

    // step 4: Validate & Cleanup
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t snaplen = snapshot ? strlen(snapshot) : 0;
    size_t buf_len = snaplen + 64;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);
    const char *extracted_first = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 0));
    double extracted_sum = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "meta_sum"));
    double extracted_compare = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "compare_result"));
    (void)extracted_first;
    (void)extracted_sum;
    (void)extracted_compare;
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}