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
//<ID> 1457
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
    cJSON *newnum = (cJSON *)0;
    cJSON *fetched_arr = (cJSON *)0;
    cJSON *first_item = (cJSON *)0;
    char *printed = (char *)0;
    double sum = 0.0;
    int validation_score = 0;
    cJSON_bool added = 0;
    cJSON_bool replaced = 0;

    // step 2: Initialize - create root object, an array, and two numeric elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(1.5);
    num1 = cJSON_CreateNumber(2.5);

    // step 3: Configure - add numbers to array and attach array to root object
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    added = cJSON_AddItemToObject(root, "values", arr);

    // step 4: Operate - fetch the array via object lookup, replace the first element, and print
    fetched_arr = cJSON_GetObjectItem(root, "values");
    first_item = cJSON_GetArrayItem(fetched_arr, 0);
    newnum = cJSON_CreateNumber(4.0);
    replaced = cJSON_ReplaceItemViaPointer(fetched_arr, first_item, newnum);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate - compute sum from array elements and derive a small validation score
    sum = cJSON_GetNumberValue(cJSON_GetArrayItem(fetched_arr, 0)) + cJSON_GetNumberValue(cJSON_GetArrayItem(fetched_arr, 1));
    validation_score = (int)(added != 0) + (int)(replaced != 0) + (int)(printed != (char *)0) + (int)cJSON_IsNumber(newnum);

    // step 6: Cleanup - free printed buffer and delete the JSON tree
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)sum;
    (void)validation_score;
    (void)num0;
    (void)num1;
    (void)added;
    (void)replaced;
    (void)fetched_arr;
    (void)first_item;
    (void)newnum;
    // API sequence test completed successfully
    return 66;
}