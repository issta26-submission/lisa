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
//<ID> 161
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *pi_item = cJSON_CreateNumber(3.14159);
    cJSON *name_item = NULL;
    cJSON *numbers_array = cJSON_CreateArray();
    cJSON *num_item1 = cJSON_CreateNumber(10.0);
    cJSON *num_item2 = cJSON_CreateNumber(20.0);
    cJSON *average_item = NULL;
    double pi_value = 0.0;
    int array_size = 0;
    double average = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "pi", pi_item);
    name_item = cJSON_AddStringToObject(root, "name", "example");
    cJSON_AddItemToArray(numbers_array, num_item1);
    cJSON_AddItemToArray(numbers_array, num_item2);
    cJSON_AddItemToObject(root, "numbers", numbers_array);

    // step 3: Operate / Validate
    pi_value = cJSON_GetNumberValue(pi_item);
    array_size = cJSON_GetArraySize(numbers_array);
    average = (pi_value + (double)array_size) / 2.0;
    average_item = cJSON_CreateNumber(average);
    cJSON_AddItemToObject(root, "average", average_item);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}