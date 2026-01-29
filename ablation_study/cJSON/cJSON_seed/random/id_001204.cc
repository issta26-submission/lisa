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
//<ID> 1204
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
    cJSON *num_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    int size = 0;
    double num_val = 0.0;
    int validation_score = 0;

    // step 2: Initialize - create root object and an array member
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", arr);

    // step 3: Configure - create and attach a number, a string, and a true boolean
    num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num_item);
    str_item = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greeting", str_item);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", true_item);

    // step 4: Operate - retrieve number from the array and obtain array size
    num_item = cJSON_GetArrayItem(arr, 0);
    num_val = cJSON_GetNumberValue(num_item);
    size = cJSON_GetArraySize(arr);

    // step 5: Validate - compute a small validation score based on retrieved values
    validation_score = (int)(num_val == 42.0) + size + (str_item != (cJSON *)0) + (true_item != (cJSON *)0);

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}