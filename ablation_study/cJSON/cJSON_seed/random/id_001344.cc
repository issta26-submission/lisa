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
//<ID> 1344
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
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    double val0 = 0.0;
    double val1 = 0.0;
    double sum = 0.0;
    int size = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object, an array and two numeric items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(10.5);
    n2 = cJSON_CreateNumber(2.5);

    // step 3: Configure - attach numbers to array and attach array and an extra number to root
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToObject(root, "extra", cJSON_CreateNumber(42.0));

    // step 4: Operate - retrieve the array, extract numeric values, compute sum and size, add results to root
    retrieved_arr = cJSON_GetObjectItem(root, "numbers");
    elem0 = cJSON_GetArrayItem(retrieved_arr, 0);
    elem1 = cJSON_GetArrayItem(retrieved_arr, 1);
    val0 = cJSON_GetNumberValue(elem0);
    val1 = cJSON_GetNumberValue(elem1);
    sum = val0 + val1;
    size = cJSON_GetArraySize(retrieved_arr);
    cJSON_AddItemToObject(root, "sum", cJSON_CreateNumber(sum));
    cJSON_AddItemToObject(root, "count", cJSON_CreateNumber((double)size));

    // step 5: Validate - compute a simple validation score from results (no branches)
    validation_score = (int)(sum == 13.0) + (int)(size == 2) + (int)(cJSON_GetNumberValue(cJSON_GetObjectItem(root, "extra")) == 42.0);
    (void)validation_score;

    // step 6: Cleanup - delete the root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}