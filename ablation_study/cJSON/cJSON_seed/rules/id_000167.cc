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
//<ID> 167
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
    cJSON *num1 = cJSON_CreateNumber(42.5);
    cJSON *num2 = NULL;
    cJSON *status_item = NULL;
    cJSON *first_item = NULL;
    double first_value = 0.0;
    int size_before = 0;
    int size_after = 0;

    // step 2: Setup / Configure
    status_item = cJSON_AddStringToObject(root, "status", "ok");
    cJSON_AddItemToObject(root, "values", array);
    cJSON_AddItemToArray(array, num1);

    // step 3: Operate / Validate
    size_before = cJSON_GetArraySize(array);
    first_item = cJSON_GetArrayItem(array, 0);
    first_value = cJSON_GetNumberValue(first_item);
    num2 = cJSON_CreateNumber(first_value + (double)size_before);
    cJSON_AddItemToArray(array, num2);
    size_after = cJSON_GetArraySize(array);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    (void)status_item;
    (void)first_item;
    (void)size_before;
    (void)size_after;
    (void)first_value;
    return 66;
}