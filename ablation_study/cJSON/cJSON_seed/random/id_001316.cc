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
//<ID> 1316
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *numbers = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    cJSON *retrieved_elem = (cJSON *)0;
    char *printed = (char *)0;
    double second_value = 0.0;
    double derived_value = 0.0;
    int validation_score = 0;

    // step 2: Initialize - create root object, an array and two numeric items
    root = cJSON_CreateObject();
    numbers = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(3.14);
    num2 = cJSON_CreateNumber(2.0);

    // step 3: Configure - add numeric items to the array and attach the array to root
    cJSON_AddItemToArray(numbers, num1);
    cJSON_AddItemToArray(numbers, num2);
    cJSON_AddItemToObject(root, "numbers", numbers);

    // step 4: Operate - retrieve the array via object lookup, extract the second element and derive a new number
    retrieved = cJSON_GetObjectItem(root, "numbers");
    retrieved_elem = cJSON_GetArrayItem(retrieved, 1);
    second_value = cJSON_GetNumberValue(retrieved_elem);
    derived_value = second_value * 2.5;
    cJSON_AddNumberToObject(root, "derived", derived_value);

    // step 5: Validate and Serialize - produce unformatted JSON and compute a simple validation score
    printed = cJSON_PrintUnformatted(root);
    validation_score = (int)(root != (cJSON *)0) + (int)(numbers != (cJSON *)0) + (int)(retrieved != (cJSON *)0) + (int)(retrieved_elem != (cJSON *)0) + (int)(printed != (char *)0);
    (void)validation_score;

    // step 6: Cleanup - free the printed buffer and delete the root object (which frees children)
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}