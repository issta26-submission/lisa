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
//<ID> 351
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(3.1415);
    cJSON *num2 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(values, num1);
    cJSON_AddItemToArray(values, num2);
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", flag);
    cJSON_bool flag_is_bool = cJSON_IsBool(flag);
    cJSON_bool flag_is_true = cJSON_IsTrue(flag);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate
    double count = (double)cJSON_GetArraySize(values);
    cJSON *count_node = cJSON_AddNumberToObject(meta, "count", count);
    cJSON *flag_true_node = cJSON_AddNumberToObject(meta, "flag_true", (double)flag_is_true);
    (void)count_node;
    (void)flag_true_node;
    char *printed = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_bool prealloc_ok = cJSON_PrintPreallocated(root, buffer, buf_len, 0);
    (void)prealloc_ok;

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}