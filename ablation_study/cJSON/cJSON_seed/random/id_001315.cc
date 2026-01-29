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
//<ID> 1315
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
    cJSON *values = (cJSON *)0;
    cJSON *first = (cJSON *)0;
    char *printed = (char *)0;
    double first_val = 0.0;
    int validation_score = 0;
    cJSON_bool added1 = (cJSON_bool)0;
    cJSON_bool added2 = (cJSON_bool)0;

    // step 2: Initialize - create root object and numeric items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(10.5);
    num2 = cJSON_CreateNumber(20.25);

    // step 3: Configure - add numbers to array and attach array to root
    added1 = cJSON_AddItemToArray(arr, num1);
    added2 = cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddNumberToObject(root, "count", 2.0);

    // step 4: Operate - retrieve the array via object lookup, extract first value and serialize
    values = cJSON_GetObjectItem(root, "values");
    first = cJSON_GetArrayItem(values, 0);
    first_val = cJSON_GetNumberValue(first);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate - simple score using presence and numeric correctness
    validation_score = (int)(root != (cJSON *)0) + (int)(values != (cJSON *)0) + (int)(first != (cJSON *)0) + (int)(printed != (char *)0) + (int)(first_val == 10.5);
    (void)validation_score;

    // step 6: Cleanup - free printed string and delete root which frees children
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}