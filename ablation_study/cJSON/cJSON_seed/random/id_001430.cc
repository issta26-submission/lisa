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
//<ID> 1430
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
    double picked_value = 0.0;
    int validation_score = 0;

    // step 2: Initialize - create root object and a numeric array
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(3.1415);
    num1 = cJSON_CreateNumber(2.7182);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToObject(root, "values", arr);

    // step 3: Operate - retrieve an array item and read its numeric value, then attach it back to root
    picked = cJSON_GetArrayItem(arr, 1);
    picked_value = cJSON_GetNumberValue((const cJSON *const)picked);
    cJSON_AddItemToObject(root, "picked", cJSON_CreateNumber(picked_value));
    printed = cJSON_PrintUnformatted((const cJSON *)root);

    // step 4: Validate - derive a simple validation score from outcomes
    validation_score = (int)(printed != (char *)0) + (int)(picked != (cJSON *)0) + (int)(picked_value > 0.0);
    (void)validation_score;

    // step 5: Cleanup - free printed buffer and delete the JSON tree
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // step 6: Finalize - record and ignore some variables to satisfy self-contained requirement
    (void)num0;
    (void)num1;
    (void)picked_value;
    // API sequence test completed successfully
    return 66;
}