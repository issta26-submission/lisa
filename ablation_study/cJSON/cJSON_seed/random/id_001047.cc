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
//<ID> 1047
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
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *str = (cJSON *)0;
    cJSON *fetched = (cJSON *)0;
    int array_size = 0;
    double fetched_value = 0.0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    cJSON_bool add_ok3 = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create a root object, an array, two numbers and a string
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(10.0);
    num2 = cJSON_CreateNumber(20.5);
    str = cJSON_CreateString("hello");

    // step 3: Configure - attach numbers into the array
    add_ok1 = cJSON_AddItemToArray(arr, num1);
    add_ok2 = cJSON_AddItemToArray(arr, num2);

    // step 4: Operate - attach array and string into the root object using case-sensitive add
    add_ok3 = cJSON_AddItemToObjectCS(root, "numbers", arr);
    cJSON_bool add_ok4 = cJSON_AddItemToObjectCS(root, "greeting", str);

    // step 5: Validate - fetch array size and read a number value from the array
    array_size = cJSON_GetArraySize(arr);
    fetched = cJSON_GetArrayItem(arr, 1);
    fetched_value = cJSON_GetNumberValue(fetched);
    validation_score = (int)(root != (cJSON *)0) + (int)(array_size == 2) + (int)(fetched_value == 20.5) + (int)(add_ok1) + (int)(add_ok2) + (int)(add_ok3) + (int)(add_ok4);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}