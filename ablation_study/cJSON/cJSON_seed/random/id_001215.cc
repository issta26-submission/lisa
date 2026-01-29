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
//<ID> 1215
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *arr = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *bool_item = (cJSON *)0;
    cJSON *first = (cJSON *)0;
    cJSON *second = (cJSON *)0;
    int array_size = 0;
    char *first_str = (char *)0;
    char *second_str = (char *)0;
    int validation_score = 0;

    // step 2: Setup - create an array container
    arr = cJSON_CreateArray();

    // step 3: Configure - create a string and a boolean and add them to the array
    str_item = cJSON_CreateString("hello_world");
    bool_item = cJSON_CreateBool(1);
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, bool_item);

    // step 4: Operate - retrieve array size and items by index, then obtain string values
    array_size = cJSON_GetArraySize(arr);
    first = cJSON_GetArrayItem(arr, 0);
    second = cJSON_GetArrayItem(arr, 1);
    first_str = cJSON_GetStringValue(first);
    second_str = cJSON_GetStringValue(second);

    // step 5: Validate - compute a small validation score from meaningful data flow
    validation_score = array_size + (int)(first_str != (char *)0) + (int)(second_str == (char *)0);

    // step 6: Cleanup - delete the array which frees attached items
    cJSON_Delete(arr);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}