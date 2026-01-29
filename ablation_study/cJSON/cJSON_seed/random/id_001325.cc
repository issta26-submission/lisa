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
//<ID> 1325
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
    cJSON *retr0 = (cJSON *)0;
    cJSON *retr1 = (cJSON *)0;
    cJSON *sum_item = (cJSON *)0;
    char *printed = (char *)0;
    double v0 = 0.0;
    double v1 = 0.0;
    double sum = 0.0;
    int validation_score = 0;

    // step 2: Initialize - create root object, an array and two numeric items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(3.25);
    n2 = cJSON_CreateNumber(4.75);

    // step 3: Configure - attach numbers to array and attach the array to root
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 4: Operate - retrieve array elements via index and compute a sum, then add sum back to object
    retr0 = cJSON_GetArrayItem(arr, 0);
    retr1 = cJSON_GetArrayItem(arr, 1);
    v0 = cJSON_GetNumberValue(retr0);
    v1 = cJSON_GetNumberValue(retr1);
    sum = v0 + v1;
    sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sum_item);

    // step 5: Validate - serialize and compute a simple validation score
    printed = cJSON_PrintUnformatted(root);
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(retr0 != (cJSON *)0) + (int)(retr1 != (cJSON *)0) + (int)(printed != (char *)0) + (int)(sum == 8.0);

    // step 6: Cleanup - free printed buffer and delete root which frees attached items
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}