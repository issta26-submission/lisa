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
//<ID> 162
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(10.5);
    cJSON *num2 = cJSON_CreateNumber(20.25);
    cJSON *sum_item = NULL;
    cJSON *first = NULL;
    cJSON *second = NULL;
    cJSON *label_item = NULL;
    double value1 = 0.0;
    double value2 = 0.0;
    double sum = 0.0;
    int arr_size = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(array, num1);
    cJSON_AddItemToArray(array, num2);
    cJSON_AddItemToObject(root, "values", array);
    label_item = cJSON_AddStringToObject(root, "label", "sample");

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(array);
    first = cJSON_GetArrayItem(array, 0);
    second = cJSON_GetArrayItem(array, 1);
    value1 = cJSON_GetNumberValue(first);
    value2 = cJSON_GetNumberValue(second);
    sum = value1 + value2;
    sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sum_item);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}