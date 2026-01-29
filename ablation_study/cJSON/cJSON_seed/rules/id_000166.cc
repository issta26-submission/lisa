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
//<ID> 166
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
    cJSON *n0 = cJSON_CreateNumber(1.0);
    cJSON *n1 = cJSON_CreateNumber(2.0);
    cJSON *n2 = cJSON_CreateNumber(3.5);
    cJSON *sum_item = NULL;
    cJSON *size_item = NULL;
    double v0 = 0.0;
    double v1 = 0.0;
    double v2 = 0.0;
    int arr_size = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(array, n0);
    cJSON_AddItemToArray(array, n1);
    cJSON_AddItemToArray(array, n2);
    cJSON_AddItemToObject(root, "values", array);
    cJSON_AddStringToObject(root, "label", "example");

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(array);
    v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(array, 0));
    v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(array, 1));
    v2 = cJSON_GetNumberValue(cJSON_GetArrayItem(array, 2));
    sum_item = cJSON_CreateNumber(v0 + v1 + v2);
    size_item = cJSON_CreateNumber((double)arr_size);
    cJSON_AddItemToObject(root, "sum", sum_item);
    cJSON_AddItemToObject(root, "count", size_item);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}