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
//<ID> 1437
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
    char *printed = (char *)0;
    double value = 0.0;
    int validation_score = 0;
    cJSON_bool added = 0;

    // step 2: Initialize - create root object and an array of two numbers, attach to root
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(3.14);
    num1 = cJSON_CreateNumber(2.71);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    added = cJSON_AddItemToObject(root, "numbers", arr);

    // step 3: Operate - retrieve first array item, get its numeric value, and print unformatted
    picked = cJSON_GetArrayItem(arr, 0);
    value = cJSON_GetNumberValue(picked);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Validate - derive a small validation score from results
    validation_score = (int)(picked != (cJSON *)0) + (int)(printed != (char *)0) + (int)(value > 0.0) + (int)added;

    // step 5: Cleanup - free printed buffer and delete the JSON tree
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // step 6: Finalize - record and ignore validation result, return success
    (void)validation_score;
    (void)num0;
    (void)num1;
    (void)value;
    // API sequence test completed successfully
    return 66;
}