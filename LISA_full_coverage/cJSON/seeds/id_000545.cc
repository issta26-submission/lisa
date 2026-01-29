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
//<ID> 545
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
    cJSON *count_created = cJSON_CreateNumber(3.0);
    cJSON_AddItemToObject(root, "count_created", count_created);
    cJSON *count_direct_ref = cJSON_AddNumberToObject(root, "count_direct", 3.0);
    (void)count_direct_ref;

    // step 2: Configure
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    cJSON_bool initial_equal = cJSON_Compare(root, dup_root, 1);
    cJSON *count_obj = cJSON_GetObjectItem(root, "count_direct");
    double count_val = cJSON_GetNumberValue(count_obj);
    double sum_len = count_val + (double)strlen(names[0]);
    cJSON *meta = cJSON_CreateObject();
    cJSON *sum_item = cJSON_CreateNumber(sum_len);
    cJSON_AddItemToObject(meta, "sum", sum_item);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate
    cJSON *detached = cJSON_DetachItemFromArray(arr, 1);
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t snaplen = snapshot ? strlen(snapshot) : 0;
    size_t buf_len = snaplen + 64;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);

    // step 4: Validate & Cleanup
    cJSON *dup_detached = cJSON_Duplicate(detached, 1);
    cJSON *added_num = cJSON_AddNumberToObject(dup_root, "extra", 7.0);
    cJSON_bool after_equal = cJSON_Compare(root, dup_root, 1);
    cJSON *meta_after = cJSON_GetObjectItem(root, "meta");
    cJSON *sum_after = cJSON_GetObjectItem(meta_after, "sum");
    double extracted_sum = cJSON_GetNumberValue(sum_after);
    (void)initial_equal;
    (void)after_equal;
    (void)extracted_sum;
    (void)added_num;
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(dup_detached);
    cJSON_Delete(detached);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}