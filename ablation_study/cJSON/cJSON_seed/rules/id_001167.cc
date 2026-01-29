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
//<ID> 1167
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = NULL;
    cJSON *numbers_array = NULL;
    cJSON *pi_item = NULL;
    cJSON *copied_pi = NULL;
    cJSON *true_item = NULL;
    cJSON *false_item = NULL;
    char *printed = NULL;
    double pi_value = 0.0;
    int array_size = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    pi_item = cJSON_CreateNumber(3.141592653589793);
    cJSON_AddItemToObject(root, "pi", pi_item);
    numbers_array = cJSON_CreateArray();
    cJSON_AddItemToArray(numbers_array, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(numbers_array, cJSON_CreateNumber(2.0));
    cJSON_AddItemToArray(numbers_array, cJSON_CreateNumber(3.0));
    cJSON_AddItemToObject(root, "numbers", numbers_array);
    true_item = cJSON_CreateTrue();
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", true_item);
    cJSON_AddItemToObject(root, "disabled", false_item);

    // step 3: Operate / Validate
    array_size = cJSON_GetArraySize(numbers_array);
    pi_value = cJSON_GetNumberValue(pi_item);
    copied_pi = cJSON_CreateNumber(pi_value);
    cJSON_AddItemToObject(root, "pi_copy", copied_pi);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}