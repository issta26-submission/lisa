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
//<ID> 163
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
    cJSON *num1 = cJSON_CreateNumber(1.0);
    cJSON *num2 = cJSON_CreateNumber(2.0);
    cJSON *num3 = cJSON_CreateNumber(3.0);
    cJSON *retrieved = NULL;
    cJSON *summary = NULL;
    double retrieved_value = 0.0;
    int array_size = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(array, num1);
    cJSON_AddItemToArray(array, num2);
    cJSON_AddItemToArray(array, num3);
    cJSON_AddItemToObject(root, "values", array);
    cJSON_AddStringToObject(root, "description", "integer sequence");

    // step 3: Operate / Validate
    array_size = cJSON_GetArraySize(array);
    retrieved = cJSON_GetArrayItem(array, 1);
    retrieved_value = cJSON_GetNumberValue(retrieved);
    summary = cJSON_CreateNumber(retrieved_value + (double)array_size);
    cJSON_AddItemToObject(root, "summary", summary);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}