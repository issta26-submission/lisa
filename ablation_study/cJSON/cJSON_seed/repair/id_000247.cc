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
//<ID> 247
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.5);
    cJSON *n3 = cJSON_CreateNumber(30.25);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddItemToArray(values, n3);
    cJSON_AddItemToObject(data, "values", values);
    cJSON_AddItemToObject(root, "data", data);

    // step 2: Configure
    cJSON *mid_item = cJSON_GetArrayItem(values, 1);
    double mid_val = cJSON_GetNumberValue(mid_item);
    cJSON_AddNumberToObject(root, "middle_value", mid_val);
    cJSON_bool has_data = cJSON_HasObjectItem(root, "data");
    cJSON_AddNumberToObject(root, "has_data_flag", (double)has_data);
    int arr_size = cJSON_GetArraySize(values);
    cJSON_AddNumberToObject(root, "array_size", (double)arr_size);

    // step 3: Operate and Validate
    cJSON *retrieved_data = cJSON_GetObjectItem(root, "data");
    cJSON *retrieved_values = cJSON_GetObjectItem(retrieved_data, "values");
    cJSON *first_item = cJSON_GetArrayItem(retrieved_values, 0);
    cJSON *last_item = cJSON_GetArrayItem(retrieved_values, 2);
    double sum = cJSON_GetNumberValue(first_item) + mid_val + cJSON_GetNumberValue(last_item);
    cJSON_AddNumberToObject(root, "sum", sum);
    cJSON *summary = cJSON_CreateString("computed");
    cJSON_AddItemToObject(root, "summary", summary);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}