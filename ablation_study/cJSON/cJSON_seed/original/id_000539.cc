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
//<ID> 539
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
    cJSON *count = cJSON_CreateNumber(3.0);
    cJSON_AddItemToObject(root, "count", count);
    const char *version = cJSON_Version();

    // step 2: Configure
    double count_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "count"));
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
    double extracted_sum = cJSON_GetNumberValue(cJSON_GetObjectItem(meta, "sum"));
    (void)version;
    (void)extracted_sum;
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}