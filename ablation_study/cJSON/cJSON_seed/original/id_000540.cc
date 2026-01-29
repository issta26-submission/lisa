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
//<ID> 540
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
    cJSON_AddNumberToObject(root, "total", 3.0);

    // step 2: Configure
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, dup_root, 1);
    cJSON_AddNumberToObject(root, "equal", (double)equal);

    // step 3: Operate
    cJSON *detached = cJSON_DetachItemFromArray(arr, 1);
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t buf_len = 256;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);

    // step 4: Validate & Cleanup
    double total_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "total"));
    double count_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "count"));
    double equal_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "equal"));
    (void)total_val;
    (void)count_val;
    (void)equal_val;
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(detached);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}