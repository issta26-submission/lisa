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
//<ID> 1458
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num0 = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *replacement_num = (cJSON *)0;
    cJSON *got_child = (cJSON *)0;
    cJSON *got_numbers = (cJSON *)0;
    char *printed = (char *)0;
    int validation_score = 0;
    cJSON_bool added_child = 0;
    cJSON_bool added_numbers = 0;
    cJSON_bool replaced = 0;

    // step 2: Initialize - create root, child object, array and numeric elements
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(7.0);
    num1 = cJSON_CreateNumber(11.0);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);

    // step 3: Configure - attach the array to the child object and attach child to root
    added_numbers = cJSON_AddItemToObject(child, "numbers", arr);
    added_child = cJSON_AddItemToObject(root, "child", child);

    // step 4: Operate - retrieve objects and replace the second number in the array via pointer
    got_child = cJSON_GetObjectItem(root, "child");
    got_numbers = cJSON_GetObjectItem(got_child, "numbers");
    replacement_num = cJSON_CreateNumber(42.0);
    replaced = cJSON_ReplaceItemViaPointer(got_numbers, num1, replacement_num);
    cJSON_AddItemToArray(got_numbers, cJSON_CreateNumber(3.14));

    // step 5: Validate - derive a small validation score from results
    validation_score = (int)(got_child != (cJSON *)0) + (int)(got_numbers != (cJSON *)0) + (int)(added_child != 0) + (int)(added_numbers != 0) + (int)(replaced != 0);

    // step 6: Cleanup - print, free printed buffer and delete the whole JSON tree
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)num0;
    (void)num1;
    (void)replacement_num;
    (void)added_child;
    (void)added_numbers;
    (void)replaced;
    (void)got_child;
    (void)got_numbers;
    // API sequence test completed successfully
    return 66;
}