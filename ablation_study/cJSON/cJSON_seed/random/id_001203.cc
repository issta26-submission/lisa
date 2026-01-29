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
//<ID> 1203
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *n1 = (cJSON *)0;
    cJSON *n2 = (cJSON *)0;
    cJSON *btrue = (cJSON *)0;
    cJSON *s = (cJSON *)0;
    cJSON *arr_ret = (cJSON *)0;
    cJSON *first_item = (cJSON *)0;
    cJSON *enabled_item = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    char *name_val = (char *)0;
    double first_num = 0.0;
    int size = 0;
    int validation_score = 0;

    // step 2: Setup - create root object, an array, two numbers, a true boolean and a string
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(3.14);
    n2 = cJSON_CreateNumber(2.0);
    btrue = cJSON_CreateTrue();
    s = cJSON_CreateString("example_name");

    // step 3: Configure - populate array and attach members to root object
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToObject(root, "enabled", btrue);
    cJSON_AddItemToObject(root, "name", s);

    // step 4: Operate - retrieve the array and its first numeric element, and retrieve the name and enabled flag
    arr_ret = cJSON_GetObjectItem(root, "values");
    size = cJSON_GetArraySize(arr_ret);
    first_item = cJSON_GetArrayItem(arr_ret, 0);
    first_num = cJSON_GetNumberValue(first_item);
    name_item = cJSON_GetObjectItem(root, "name");
    name_val = cJSON_GetStringValue(name_item);
    enabled_item = cJSON_GetObjectItem(root, "enabled");

    // step 5: Validate - compute a small validation score based on retrieved values
    validation_score = size + (first_num == 3.14) + (name_val != (char *)0) + (int)cJSON_IsTrue(enabled_item);

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}