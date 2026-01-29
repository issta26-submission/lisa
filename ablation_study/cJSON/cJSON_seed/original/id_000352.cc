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
//<ID> 352
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateArray();
    cJSON *meta = cJSON_CreateObject();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *num1 = cJSON_CreateNumber(10.0);
    cJSON *num2 = cJSON_CreateNumber(20.5);
    cJSON *num3 = cJSON_CreateNumber(30.25);

    // step 2: Configure
    cJSON_AddItemToArray(data, num1);
    cJSON_AddItemToArray(data, num2);
    cJSON_AddItemToArray(data, num3);
    cJSON_AddItemToArray(data, true_item);
    cJSON_AddItemToObject(root, "data", data);
    cJSON_AddNumberToObject(root, "first_value", 10.0);
    cJSON_AddNumberToObject(root, "sum", 60.75);

    // step 3: Operate
    cJSON_bool is_bool = cJSON_IsBool(true_item);
    cJSON_bool is_true = cJSON_IsTrue(true_item);
    cJSON_AddNumberToObject(meta, "is_bool", (double)is_bool);
    cJSON_AddNumberToObject(meta, "is_true", (double)is_true);
    cJSON_AddItemToObject(root, "meta", meta);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON_bool prealloc_ok = cJSON_PrintPreallocated(root, buffer, 256, 0);
    cJSON *pre_flag = cJSON_CreateBool(prealloc_ok);
    cJSON_AddItemToObject(root, "prealloc_ok", pre_flag);

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}