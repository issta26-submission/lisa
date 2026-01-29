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
//<ID> 1319
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
    cJSON *n1 = (cJSON *)0;
    cJSON *n2 = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    char *printed = (char *)0;
    double val0 = 0.0;
    double val1 = 0.0;
    double sum = 0.0;
    int validation_score = 0;

    // step 2: Initialize - create root object, an array and two numeric items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(1.5);
    n2 = cJSON_CreateNumber(2.5);

    // step 3: Configure - attach numbers to array and array to root
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddNumberToObject(root, "count", 2.0);

    // step 4: Operate - retrieve the array via object lookup and extract numeric values
    retrieved_arr = cJSON_GetObjectItem(root, "values");
    elem0 = cJSON_GetArrayItem(retrieved_arr, 0);
    elem1 = cJSON_GetArrayItem(retrieved_arr, 1);
    val0 = cJSON_GetNumberValue(elem0);
    val1 = cJSON_GetNumberValue(elem1);
    sum = val0 + val1;
    cJSON_AddNumberToObject(root, "sum", sum);

    // step 5: Validate - serialize unformatted and compute a simple validation score
    printed = cJSON_PrintUnformatted(root);
    validation_score = (int)(root != (cJSON *)0) + (int)(retrieved_arr != (cJSON *)0) + (int)(printed != (char *)0) + (int)(sum == 4.0);

    // step 6: Cleanup - free printed buffer and delete root
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}