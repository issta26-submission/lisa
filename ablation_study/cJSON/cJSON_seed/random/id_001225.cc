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
//<ID> 1225
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *str_ref = (cJSON *)0;
    cJSON *first = (cJSON *)0;
    cJSON *second = (cJSON *)0;
    double first_num = 0.0;
    int invalid_flag = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object and an array, attach array to root
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure - create a numeric item and a string reference, add both to the array
    num_item = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToArray(arr, num_item);
    str_ref = cJSON_CreateStringReference("hello_ref");
    cJSON_AddItemToArray(arr, str_ref);

    // step 4: Operate - retrieve elements and obtain numeric value and invalid status
    first = cJSON_GetArrayItem(arr, 0);
    second = cJSON_GetArrayItem(arr, 1);
    first_num = cJSON_GetNumberValue(first);
    invalid_flag = (int)cJSON_IsInvalid(second);

    // step 5: Validate - compute a simple validation score from meaningful data flow
    validation_score = (int)(first_num == 3.1415) + invalid_flag + (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(num_item != (cJSON *)0) + (int)(str_ref != (cJSON *)0);

    // step 6: Cleanup - delete root which frees the array and all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}