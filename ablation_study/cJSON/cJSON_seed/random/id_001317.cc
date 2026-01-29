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
//<ID> 1317
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
    cJSON *retrieved = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    char *printed = (char *)0;
    double first_value = 0.0;
    double derived_value = 0.0;
    int validation_score = 0;

    // step 2: Initialize - create root object, an array and two numeric items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(3.14);
    num2 = cJSON_CreateNumber(2.0);

    // step 3: Configure - populate the array and attach it to the root object
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "nums", arr);

    // step 4: Operate - retrieve the array via object lookup, read the first number and add a derived number
    retrieved = cJSON_GetObjectItem(root, "nums");
    first_elem = cJSON_GetArrayItem(retrieved, 0);
    first_value = cJSON_GetNumberValue(first_elem);
    derived_value = first_value * 2.0;
    cJSON_AddItemToArray(retrieved, cJSON_CreateNumber(derived_value));

    // step 5: Validate - serialize unformatted and compute a small validation score
    printed = cJSON_PrintUnformatted(root);
    validation_score = (int)(root != (cJSON *)0) + (int)(retrieved != (cJSON *)0) + (int)(printed != (char *)0) + (int)(first_value == 3.14);
    (void)validation_score;

    // step 6: Cleanup - free serialized string and delete the root (which frees children)
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}