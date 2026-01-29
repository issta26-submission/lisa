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
//<ID> 854
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
    cJSON *detached = (cJSON *)0;
    cJSON *par = (cJSON *)0;
    cJSON *par_x = (cJSON *)0;
    cJSON *par_x_first = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_num1 = 0;
    cJSON_bool added_num2 = 0;
    cJSON_bool added_arr_to_root = 0;
    cJSON_bool added_detached = 0;
    cJSON_bool is_detached_number = 0;
    cJSON_bool is_par_first_number = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(1.0);
    num2 = cJSON_CreateNumber(2.0);

    // step 3: Configure
    added_num1 = cJSON_AddItemToArray(arr, num1);
    added_num2 = cJSON_AddItemToArray(arr, num2);
    added_arr_to_root = cJSON_AddItemToObject(root, "arr", arr);

    // step 4: Operate
    detached = cJSON_DetachItemFromArray(arr, 0);
    added_detached = cJSON_AddItemToObject(root, "detached", detached);
    par = cJSON_Parse("{\"x\":[3.14,7]}");
    par_x = cJSON_GetObjectItem(par, "x");
    par_x_first = cJSON_GetArrayItem(par_x, 0);
    is_detached_number = cJSON_IsNumber(detached);
    is_par_first_number = cJSON_IsNumber(par_x_first);
    printed = cJSON_Print(root);

    // step 5: Validate
    validation_score = (int)added_num1 + (int)added_num2 + (int)added_arr_to_root + (int)added_detached + (printed != (char *)0) + (par != (cJSON *)0) + (int)is_detached_number + (int)is_par_first_number;
    (void)validation_score;
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(par);

    // API sequence test completed successfully
    return 66;
}