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
//<ID> 1434
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
    cJSON *num0 = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *picked = (cJSON *)0;
    cJSON *sumNode = (cJSON *)0;
    char *printed = (char *)0;
    double value = 0.0;
    int validation_score = 0;

    // step 2: Initialize - create root object, an array and two numeric elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(3.14);
    num1 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToObject(root, "values", arr);

    // step 3: Operate - pick an array item, extract its numeric value, create a summary and print unformatted
    picked = cJSON_GetArrayItem(arr, 1);
    value = cJSON_GetNumberValue(picked);
    sumNode = cJSON_CreateNumber(value + cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0)));
    cJSON_AddItemToObject(root, "sum", sumNode);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Validate - derive a small validation score from extracted value and print result presence
    validation_score = (int)value + (int)(printed != (char *)0);

    // step 5: Cleanup - free printed buffer and delete the JSON tree
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // step 6: Finalize - silence validation result
    (void)validation_score;
    // API sequence test completed successfully
    return 66;
}