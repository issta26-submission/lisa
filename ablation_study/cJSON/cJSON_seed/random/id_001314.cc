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
//<ID> 1314
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
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *first_item = (cJSON *)0;
    char *printed = (char *)0;
    double first_value = 0.0;
    double scaled_value = 0.0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Initialize - create root object, an array and numeric items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(42.0);
    num2 = cJSON_CreateNumber(3.14);

    // step 3: Configure - populate the array and attach it to the root
    add_ok1 = cJSON_AddItemToArray(arr, num1);
    add_ok2 = cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddNumberToObject(root, "multiplier", 2.0);

    // step 4: Operate - retrieve the array, extract the first number and compute a scaled value
    retrieved_arr = cJSON_GetObjectItem(root, "values");
    first_item = cJSON_GetArrayItem(retrieved_arr, 0);
    first_value = cJSON_GetNumberValue(first_item);
    scaled_value = first_value * cJSON_GetNumberValue(cJSON_GetObjectItem(root, "multiplier"));
    cJSON_AddNumberToObject(root, "scaled_first", scaled_value);

    // step 5: Validate - serialize unformatted and compute a simple validation score
    printed = cJSON_PrintUnformatted(root);
    validation_score = (int)(root != (cJSON *)0) + (int)(retrieved_arr != (cJSON *)0) + (int)(first_item != (cJSON *)0) + (int)(printed != (char *)0) + (int)(add_ok1 != 0) + (int)(add_ok2 != 0);
    (void)validation_score;

    // step 6: Cleanup - free printed buffer and delete the root object
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}