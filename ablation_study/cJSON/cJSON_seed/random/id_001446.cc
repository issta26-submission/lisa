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
//<ID> 1446
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
    cJSON *orphan = (cJSON *)0;
    cJSON *sum_node = (cJSON *)0;
    cJSON *picked0 = (cJSON *)0;
    cJSON *picked1 = (cJSON *)0;
    cJSON_bool added0 = 0;
    cJSON_bool added1 = 0;
    cJSON_bool added_arr = 0;
    double v0 = 0.0;
    double v1 = 0.0;
    int validation_score = 0;

    // step 2: Initialize - create root object and an array
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    added_arr = cJSON_AddItemToObject(root, "values", arr);

    // step 3: Configure - create numeric nodes and an orphan, attach numerics to array and add a descriptive string
    num0 = cJSON_CreateNumber(10.0);
    num1 = cJSON_CreateNumber(25.5);
    orphan = cJSON_CreateNumber(7.75);
    added0 = cJSON_AddItemToArray(arr, num0);
    added1 = cJSON_AddItemToArray(arr, num1);
    cJSON_AddStringToObject(root, "description", "array of two numbers");

    // step 4: Operate - read back the numbers from the array, compute their sum, and attach the sum to root
    picked0 = cJSON_GetArrayItem(arr, 0);
    picked1 = cJSON_GetArrayItem(arr, 1);
    v0 = cJSON_GetNumberValue(picked0);
    v1 = cJSON_GetNumberValue(picked1);
    sum_node = cJSON_CreateNumber(v0 + v1);
    cJSON_AddItemToObject(root, "sum", sum_node);

    // step 5: Validate - derive a small validation score from attachments and values
    validation_score = (int)added_arr + (int)added0 + (int)added1 + (int)((v0 + v1) == cJSON_GetNumberValue(sum_node));

    // step 6: Cleanup - delete the orphan (not attached) and then delete the whole JSON tree
    cJSON_Delete(orphan);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)picked0;
    (void)picked1;
    return 66;
}