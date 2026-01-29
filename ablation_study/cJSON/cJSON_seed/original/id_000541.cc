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
//<ID> 541
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *names[] = { "alpha", "beta", "gamma" };
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateStringArray(names, 3);
    cJSON_AddItemToObject(root, "names", arr);

    // step 2: Configure
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", num_item);
    cJSON *count_added = cJSON_AddNumberToObject(root, "count", 3.0);
    cJSON *dup_arr = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "names_copy", dup_arr);

    // step 3: Operate
    cJSON_bool equal = cJSON_Compare(arr, dup_arr, 1);
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t snaplen = snapshot ? strlen(snapshot) : 0;
    size_t buf_len = snaplen + 64;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);

    // step 4: Validate & Cleanup
    double answer_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "answer"));
    double count_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "count"));
    int names_size = cJSON_GetArraySize(cJSON_GetObjectItem(root, "names"));
    (void)equal;
    (void)answer_val;
    (void)count_val;
    (void)names_size;
    (void)count_added;
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}