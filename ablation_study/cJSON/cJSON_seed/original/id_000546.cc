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
//<ID> 546
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

    // step 2: Configure
    double count_val = 3.0;
    cJSON *count_item = cJSON_AddNumberToObject(root, "count", count_val);
    double sum_len = count_val + (double)strlen(names[0]);
    cJSON *meta = cJSON_CreateObject();
    cJSON *sum_item_created = cJSON_CreateNumber(sum_len);
    cJSON_AddItemToObject(meta, "sum", sum_item_created);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, dup, 1);
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t snaplen = snapshot ? strlen(snapshot) : 0;
    size_t buf_len = snaplen + 64;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);
    cJSON *detached = cJSON_DetachItemFromArray(arr, 1);
    const char *detached_val = cJSON_GetStringValue(detached);

    // step 4: Validate & Cleanup
    double extracted_sum = cJSON_GetNumberValue(cJSON_GetObjectItem(meta, "sum"));
    (void)extracted_sum;
    (void)detached_val;
    (void)equal;
    (void)count_item;
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(detached);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}