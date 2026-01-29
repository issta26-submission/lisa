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
//<ID> 1343
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
    cJSON *n3 = (cJSON *)0;
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    cJSON *elem2 = (cJSON *)0;
    double v0 = 0.0;
    double v1 = 0.0;
    double v2 = 0.0;
    double sum = 0.0;
    double avg = 0.0;
    int arr_size = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object, an array and three numeric items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(1.5);
    n2 = cJSON_CreateNumber(2.5);
    n3 = cJSON_CreateNumber(3.0);

    // step 3: Configure - attach numeric items to the array and attach array to root via AddItemToObject
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, n3);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToObject(root, "expected_count", cJSON_CreateNumber(3.0));

    // step 4: Operate - retrieve array size and numeric values, compute sum and average
    arr_size = cJSON_GetArraySize(arr);
    elem0 = cJSON_GetArrayItem(arr, 0);
    elem1 = cJSON_GetArrayItem(arr, 1);
    elem2 = cJSON_GetArrayItem(arr, 2);
    v0 = cJSON_GetNumberValue(elem0);
    v1 = cJSON_GetNumberValue(elem1);
    v2 = cJSON_GetNumberValue(elem2);
    sum = v0 + v1 + v2;
    avg = sum / (double)arr_size;
    cJSON_AddItemToObject(root, "sum", cJSON_CreateNumber(sum));
    cJSON_AddItemToObject(root, "average", cJSON_CreateNumber(avg));

    // step 5: Validate - produce a small validation score from observed results
    validation_score = (int)(arr_size == 3) + (int)(sum == 7.0) + (int)(avg == (7.0 / 3.0));
    (void)validation_score;

    // step 6: Cleanup - delete the root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}