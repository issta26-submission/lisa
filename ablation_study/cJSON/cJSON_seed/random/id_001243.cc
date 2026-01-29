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
//<ID> 1243
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const int numbers[4] = {10, 20, 30, 40};
    const int numbers_count = 4;
    cJSON *int_array = (cJSON *)0;
    cJSON *work_array = (cJSON *)0;
    cJSON *first_item = (cJSON *)0;
    cJSON *dup_item = (cJSON *)0;
    cJSON_bool arr_check = 0;
    int size_int_array = 0;
    int size_work_array = 0;
    int validation_score = 0;

    // step 2: Initialize - create an int array from C ints and an empty target array
    int_array = cJSON_CreateIntArray(numbers, numbers_count);
    work_array = cJSON_CreateArray();

    // step 3: Configure - verify type and extract first element
    arr_check = cJSON_IsArray(int_array);
    first_item = cJSON_GetArrayItem(int_array, 0);

    // step 4: Operate - duplicate the first element and move the duplicate into the work array, then remove the second element from the original array
    dup_item = cJSON_Duplicate(first_item, 0);
    cJSON_AddItemToArray(work_array, dup_item);
    cJSON_DeleteItemFromArray(int_array, 1);

    // step 5: Validate and Cleanup - compute sizes, a small validation score, and free resources
    size_int_array = cJSON_GetArraySize(int_array);
    size_work_array = cJSON_GetArraySize(work_array);
    validation_score = (int)arr_check + size_int_array + size_work_array + (int)(first_item != (cJSON *)0) + (int)(dup_item != (cJSON *)0);
    (void)validation_score;
    cJSON_Delete(int_array);
    cJSON_Delete(work_array);

    // API sequence test completed successfully
    return 66;
}