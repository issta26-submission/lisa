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
//<ID> 169
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(10.5);
    cJSON *num2 = cJSON_CreateNumber(20.25);
    cJSON *sum_item = NULL;
    cJSON *first = NULL;
    cJSON *second = NULL;
    double first_val = 0.0;
    double second_val = 0.0;
    double sum_val = 0.0;
    int arr_size = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddStringToObject(root, "label", "example");

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(cJSON_GetObjectItem(root, "values"));
    first = cJSON_GetArrayItem(arr, 0);
    second = cJSON_GetArrayItem(arr, 1);
    first_val = cJSON_GetNumberValue(first);
    second_val = cJSON_GetNumberValue(second);
    sum_val = first_val + second_val;
    sum_item = cJSON_CreateNumber(sum_val);
    cJSON_AddItemToObject(root, "sum", sum_item);
    (void)cJSON_GetNumberValue(cJSON_GetObjectItem(root, "sum"));
    (void)arr_size;

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}