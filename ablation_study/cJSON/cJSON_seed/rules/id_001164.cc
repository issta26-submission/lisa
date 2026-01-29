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
//<ID> 1164
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *values_array = NULL;
    cJSON *first_num = NULL;
    cJSON *second_num = NULL;
    cJSON *copy_num = NULL;
    char *printed = NULL;
    double first_value = 0.0;
    double second_value = 0.0;
    int array_size = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "enabled", cJSON_CreateTrue());
    cJSON_AddItemToObject(root, "disabled", cJSON_CreateFalse());
    values_array = cJSON_CreateArray();
    cJSON_AddItemToArray(values_array, cJSON_CreateNumber(3.14159));
    cJSON_AddItemToArray(values_array, cJSON_CreateNumber(2.71828));
    cJSON_AddItemToObject(root, "numbers", values_array);

    // step 3: Operate / Validate
    array_size = cJSON_GetArraySize(values_array);
    first_num = cJSON_GetArrayItem(values_array, 0);
    second_num = cJSON_GetArrayItem(values_array, 1);
    first_value = cJSON_GetNumberValue(first_num);
    second_value = cJSON_GetNumberValue(second_num);
    copy_num = cJSON_CreateNumber(first_value + second_value);
    cJSON_AddItemToObject(root, "sum", copy_num);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}