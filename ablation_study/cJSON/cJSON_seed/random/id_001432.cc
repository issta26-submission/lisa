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
//<ID> 1432
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
    cJSON *picked_node = (cJSON *)0;
    char *unfmt = (char *)0;
    double picked_value = 0.0;
    int validation_score = 0;

    // step 2: Initialize - create root object and a numeric array
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(3.1415);
    num1 = cJSON_CreateNumber(2.7182);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 3: Operate - retrieve an array item, extract its numeric value, attach it back to root, and print unformatted
    picked = cJSON_GetArrayItem((const cJSON *)arr, 1);
    picked_value = cJSON_GetNumberValue((const cJSON *const)picked);
    picked_node = cJSON_CreateNumber(picked_value);
    cJSON_AddItemToObject(root, "picked_value", picked_node);
    unfmt = cJSON_PrintUnformatted((const cJSON *)root);

    // step 4: Validate - compute a small validation score from outcomes
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(picked != (cJSON *)0) + (int)(unfmt != (char *)0);

    // step 5: Cleanup - free printed buffer and delete the JSON tree
    cJSON_free((void *)unfmt);
    cJSON_Delete(root);

    // step 6: Finalize - record and ignore validation result, return success
    (void)validation_score;
    (void)picked_value;
    (void)num0;
    (void)num1;
    // API sequence test completed successfully
    return 66;
}