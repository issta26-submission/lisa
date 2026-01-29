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
//<ID> 248
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
    cJSON *num_pi = cJSON_CreateNumber(3.1415);
    cJSON *num_e = cJSON_CreateNumber(2.71828);
    cJSON_AddItemToArray(arr, num_pi);
    cJSON_AddItemToArray(arr, num_e);
    cJSON_AddItemToObject(root, "values", arr);

    // step 2: Configure
    int arr_size = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "count", (double)arr_size);
    cJSON_AddStringToObject(root, "description", "mathematical constants");

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON_bool has_values = cJSON_HasObjectItem(root, "values");
    cJSON_AddNumberToObject(root, "first_value", first_val);
    cJSON_AddNumberToObject(root, "has_values", (double)has_values);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}