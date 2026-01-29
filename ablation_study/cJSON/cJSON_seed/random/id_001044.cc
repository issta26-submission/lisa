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
//<ID> 1044
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
    cJSON *str_item = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *fetched0 = (cJSON *)0;
    int arr_size = 0;
    double first_value = 0.0;
    cJSON_bool add_ok_arr = (cJSON_bool)0;
    cJSON_bool add_ok_str = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create object, array, string and number items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("example");
    num1 = cJSON_CreateNumber(1.5);
    num2 = cJSON_CreateNumber(2.5);

    // step 3: Operate - attach array and string to root and push numbers into array
    add_ok_str = cJSON_AddItemToObjectCS(root, "name", str_item);
    add_ok_arr = cJSON_AddItemToObjectCS(root, "values", arr);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);

    // step 4: Validate - read back array size and first number value
    arr_size = cJSON_GetArraySize(arr);
    fetched0 = cJSON_GetArrayItem(arr, 0);
    first_value = cJSON_GetNumberValue(fetched0);

    // step 5: Cleanup - compute a small validation score and free all
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(str_item != (cJSON *)0) + (int)(add_ok_arr) + (int)(add_ok_str) + (int)(arr_size == 2) + (int)(first_value == 1.5);
    (void)validation_score;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}