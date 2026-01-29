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
//<ID> 367
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double numbers[4] = {1.1, 2.2, 3.3, 4.4};
    cJSON *values = cJSON_CreateDoubleArray(numbers, 4);
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    int count = cJSON_GetArraySize(values);
    cJSON *item0 = cJSON_GetArrayItem(values, 0);
    cJSON *item1 = cJSON_GetArrayItem(values, 1);
    cJSON *item2 = cJSON_GetArrayItem(values, 2);
    double v0 = cJSON_GetNumberValue(item0);
    double v1 = cJSON_GetNumberValue(item1);
    double v2 = cJSON_GetNumberValue(item2);
    cJSON *double_of_third = cJSON_CreateNumber(v2 * 2.0);
    cJSON_AddItemToObject(root, "double_of_third", double_of_third);

    // step 3: Operate and Validate
    double sum_first_two = v0 + v1;
    cJSON *sum_item = cJSON_CreateNumber(sum_first_two);
    cJSON_AddItemToObject(root, "sum_first_two", sum_item);
    int size_after_ops = cJSON_GetArraySize(values);
    cJSON *third_again = cJSON_GetArrayItem(values, 2);

    // step 4: Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}