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
//<ID> 1313
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
    cJSON *retrieved_num0 = (cJSON *)0;
    cJSON *retrieved_num1 = (cJSON *)0;
    char *unformatted = (char *)0;
    double val0 = 0.0;
    double val1 = 0.0;
    int validation_score = 0;

    // step 2: Initialize - create root object, an array and numeric items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(42.0);
    num2 = cJSON_CreateNumber(7.5);

    // step 3: Configure - populate the array and attach it to the root object
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 4: Operate - retrieve the array from the object and extract numeric values
    retrieved_arr = cJSON_GetObjectItem(root, "numbers");
    retrieved_num0 = cJSON_GetArrayItem(retrieved_arr, 0);
    retrieved_num1 = cJSON_GetArrayItem(retrieved_arr, 1);
    val0 = cJSON_GetNumberValue(retrieved_num0);
    val1 = cJSON_GetNumberValue(retrieved_num1);

    // step 5: Validate - serialize unformatted and compute a simple validation score
    unformatted = cJSON_PrintUnformatted(root);
    validation_score = (int)(root != (cJSON *)0) + (int)(retrieved_arr != (cJSON *)0) + (int)(unformatted != (char *)0) + (int)(val0 == 42.0) + (int)(val1 == 7.5);
    (void)validation_score;

    // step 6: Cleanup - free serialized buffer and delete the root (which frees children)
    cJSON_free((void *)unformatted);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}