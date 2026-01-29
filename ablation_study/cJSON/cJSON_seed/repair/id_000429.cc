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
//<ID> 429
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *data = cJSON_CreateObject();
    cJSON *title = cJSON_CreateString("TestDocument");
    cJSON *author = cJSON_CreateString("alice@example.com");
    cJSON_AddItemToObject(root, "title", title);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "data", data);
    cJSON_AddItemToObject(meta, "author", author);
    cJSON_AddNumberToObject(meta, "version", 1.2);

    // step 2: Configure
    cJSON_AddNumberToObject(data, "a", 10.0);
    cJSON_AddNumberToObject(data, "b", 20.5);
    cJSON_AddNumberToObject(data, "c", 5.25);
    cJSON_AddNumberToObject(root, "configured_flag", 1.0);
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    cJSON_AddNumberToObject(root, "is_object", (double)root_is_obj);

    // step 3: Operate
    cJSON *data_from_root = cJSON_GetObjectItem(root, "data");
    cJSON *a_item = cJSON_GetObjectItem(data_from_root, "a");
    cJSON *b_item = cJSON_GetObjectItem(data_from_root, "b");
    cJSON *c_item = cJSON_GetObjectItem(data_from_root, "c");
    double a_val = cJSON_GetNumberValue(a_item);
    double b_val = cJSON_GetNumberValue(b_item);
    double c_val = cJSON_GetNumberValue(c_item);
    double total = a_val + b_val + c_val;
    cJSON_AddNumberToObject(root, "sum", total);
    cJSON *sum_item = cJSON_GetObjectItem(root, "sum");
    double retrieved_sum = cJSON_GetNumberValue(sum_item);

    // step 4: Validate and Cleanup
    (void)retrieved_sum;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}