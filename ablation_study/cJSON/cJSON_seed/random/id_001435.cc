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
//<ID> 1435
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
    cJSON *summary = (cJSON *)0;
    char *printed = (char *)0;
    double picked_value = 0.0;
    int validation_score = 0;
    cJSON_bool added_values = 0;
    cJSON_bool added_summary = 0;

    // step 2: Setup - create root object and an array with two numeric elements, attach array to root
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(3.14);
    num1 = cJSON_CreateNumber(2.71);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    added_values = cJSON_AddItemToObject(root, "values", arr);

    // step 3: Operate - pick an array element, read its numeric value, create a derived summary and attach to root, then print
    picked = cJSON_GetArrayItem(arr, 1);
    picked_value = cJSON_GetNumberValue((const cJSON *const)picked);
    summary = cJSON_CreateNumber(picked_value + 1.0);
    added_summary = cJSON_AddItemToObject(root, "summary", summary);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Validate - derive a small validation score from results
    validation_score = (int)picked_value + (int)added_values + (int)added_summary + (int)(printed != (char *)0);
    (void)validation_score;

    // step 5: Cleanup - free printed buffer and delete the JSON tree
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}