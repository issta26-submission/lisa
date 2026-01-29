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
//<ID> 1310
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
    cJSON *sum_item = (cJSON *)0;
    char *printed = (char *)0;
    double val0 = 0.0;
    double val1 = 0.0;
    int validation_score = 0;

    // step 2: Initialize - create root object, an array and two numeric items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(42.5);
    num2 = cJSON_CreateNumber(-3.25);

    // step 3: Configure - populate the array and attach it to the root object
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "values", arr);

    // step 4: Operate - retrieve the array via object lookup, extract numbers and add their sum to root
    retrieved_arr = cJSON_GetObjectItem(root, "values");
    retrieved_num0 = cJSON_GetArrayItem(retrieved_arr, 0);
    retrieved_num1 = cJSON_GetArrayItem(retrieved_arr, 1);
    val0 = cJSON_GetNumberValue(retrieved_num0);
    val1 = cJSON_GetNumberValue(retrieved_num1);
    sum_item = cJSON_CreateNumber(val0 + val1);
    cJSON_AddItemToObject(root, "sum", sum_item);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate - form a simple validation score from results
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(retrieved_arr != (cJSON *)0) + (int)(printed != (char *)0) + (int)(val0 == 42.5) + (int)(val1 == -3.25);
    (void)validation_score;

    // step 6: Cleanup - free printed buffer and delete the root (frees attached items)
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}