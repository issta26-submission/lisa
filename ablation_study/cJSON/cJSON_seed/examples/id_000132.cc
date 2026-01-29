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
//<ID> 132
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array_holder = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "array_holder", array_holder);

    // step 2: Configure
    double values[] = {1.5, -2.0, 3.14159};
    cJSON *double_arr = cJSON_CreateDoubleArray(values, 3);
    cJSON_AddItemToObject(root, "double_array", double_arr);
    cJSON *count_num = cJSON_AddNumberToObject(root, "count", 3.0);
    cJSON *extra_num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(array_holder, extra_num);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(double_arr, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON_AddNumberToObject(root, "first_value", first_val);
    int arr_size = cJSON_GetArraySize(double_arr);
    cJSON_AddNumberToObject(root, "reported_count", (double)arr_size);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}