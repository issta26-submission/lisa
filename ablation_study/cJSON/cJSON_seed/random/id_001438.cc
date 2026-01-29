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
//<ID> 1438
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
    cJSON_bool add_result = 0;

    // step 2: Initialize - create root object, an array and two numeric elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(1.5);
    num1 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    add_result = cJSON_AddItemToObject(root, "values", arr);

    // step 3: Configure - attach a simple numeric summary to root
    cJSON *summary = cJSON_CreateNumber(2.0);
    cJSON_AddItemToObject(root, "summary_count", summary);

    // step 4: Operate - pick an array element, read its numeric value and print unformatted JSON
    picked = cJSON_GetArrayItem((const cJSON *)arr, 1);
    picked_value = cJSON_GetNumberValue((const cJSON *const)picked);
    printed = cJSON_PrintUnformatted((const cJSON *)root);

    // step 5: Validate - compute a simple validation score from results
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(picked != (cJSON *)0) + (int)(printed != (char *)0) + (int)(picked_value == 2.0) + (int)add_result;

    // step 6: Cleanup - free printed buffer and delete the JSON tree
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    (void)validation_score;
    (void)num0;
    (void)num1;
    (void)summary;
    (void)picked_value;
    // API sequence test completed successfully
    return 66;
}