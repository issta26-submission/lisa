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
//<ID> 513
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
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON *n3 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, n3);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", flag_false);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *name = cJSON_CreateString("sensor-A");
    cJSON_AddItemToObject(root, "name", name);

    // step 2: Configure
    cJSON *confirmed = cJSON_AddTrueToObject(meta, "confirmed");
    cJSON *threshold = cJSON_CreateNumber(42.5);
    cJSON_AddItemToObject(meta, "threshold", threshold);

    // step 3: Operate
    cJSON_DeleteItemFromArray(arr, 1);
    int count = cJSON_GetArraySize(arr);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first);
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t buf_len = 64 + (size_t)count;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);
    cJSON_bool was_false = cJSON_IsFalse(flag_false);
    cJSON_bool is_confirmed = cJSON_IsTrue(confirmed);

    // step 4: Validate & Cleanup
    (void)first_val;
    (void)was_false;
    (void)is_confirmed;
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}