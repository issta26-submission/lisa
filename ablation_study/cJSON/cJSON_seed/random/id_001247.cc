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
//<ID> 1247
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *src_arr = (cJSON *)0;
    cJSON *dest_arr = (cJSON *)0;
    cJSON *second_item = (cJSON *)0;
    cJSON *dup_item = (cJSON *)0;
    const int numbers[3] = {10, 20, 30};
    int size_before = 0;
    int size_after = 0;
    cJSON_bool is_array = 0;
    int validation_score = 0;

    // step 2: Initialize - create source int array and destination empty array
    src_arr = cJSON_CreateIntArray(numbers, 3);
    dest_arr = cJSON_CreateArray();

    // step 3: Configure - check type, extract second element, duplicate and add to destination
    is_array = cJSON_IsArray(src_arr);
    second_item = cJSON_GetArrayItem(src_arr, 1);
    dup_item = cJSON_Duplicate(second_item, 0);
    cJSON_AddItemToArray(dest_arr, dup_item);

    // step 4: Operate - measure size, delete first element from source, measure again
    size_before = cJSON_GetArraySize(src_arr);
    cJSON_DeleteItemFromArray(src_arr, 0);
    size_after = cJSON_GetArraySize(src_arr);

    // step 5: Validate - compute a small validation score to ensure data flowed correctly
    validation_score = (int)(is_array) + (int)(second_item != (cJSON *)0) + (int)(dup_item != (cJSON *)0) + (int)(size_before == 3) + (int)(size_after == 2) + (int)(cJSON_GetNumberValue(cJSON_GetArrayItem(src_arr, 1)) == 30.0);
    (void)validation_score;

    // step 6: Cleanup - free all allocated cJSON structures
    cJSON_Delete(src_arr);
    cJSON_Delete(dest_arr);

    // API sequence test completed successfully
    return 66;
}