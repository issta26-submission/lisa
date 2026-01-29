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
//<ID> 1242
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *int_arr = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *first_item = (cJSON *)0;
    cJSON *third_item = (cJSON *)0;
    int base_numbers[3];
    memset(base_numbers, 0, sizeof(base_numbers));
    base_numbers[0] = 10;
    base_numbers[1] = 20;
    base_numbers[2] = 30;
    int validation_score = 0;
    cJSON_bool is_array = 0;

    // step 2: Initialize - create an int array from C integers
    int_arr = cJSON_CreateIntArray(base_numbers, 3);

    // step 3: Configure - create an extra number and append it to the array
    num_item = cJSON_CreateNumber(99.0);
    cJSON_AddItemToArray(int_arr, num_item);

    // step 4: Operate - verify type and extract items by index
    is_array = cJSON_IsArray(int_arr);
    first_item = cJSON_GetArrayItem(int_arr, 0);
    third_item = cJSON_GetArrayItem(int_arr, 2);

    // step 5: Modify - remove the original middle element (index 1)
    cJSON_DeleteItemFromArray(int_arr, 1);

    // step 6: Validate and Cleanup - compute a simple validation score and free resources
    validation_score = (int)(int_arr != (cJSON *)0) + (int)(num_item != (cJSON *)0) + (int)is_array + (int)(first_item != (cJSON *)0) + (int)(third_item != (cJSON *)0);
    (void)validation_score;
    cJSON_Delete(int_arr);

    // API sequence test completed successfully
    return 66;
}