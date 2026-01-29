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
//<ID> 1450
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
    cJSON *new_num = (cJSON *)0;
    cJSON *old_node = (cJSON *)0;
    cJSON *replacement_str = (cJSON *)0;
    cJSON *obj_values = (cJSON *)0;
    char *printed = (char *)0;
    int validation_score = 0;
    cJSON_bool added_arr = 0;
    cJSON_bool added_obj = 0;
    cJSON_bool replaced_arr = 0;
    cJSON_bool replaced_obj = 0;

    // step 2: Initialize - create root object, an array and two numeric elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(1.5);
    num1 = cJSON_CreateNumber(2.5);
    added_arr = cJSON_AddItemToArray(arr, num0);
    added_arr = (cJSON_bool)(added_arr + cJSON_AddItemToArray(arr, num1));

    // step 3: Configure - attach the array to the root and add an extra node that will be replaced
    added_obj = cJSON_AddItemToObject(root, "values", arr);
    old_node = cJSON_CreateNumber(42.0);
    added_obj = (cJSON_bool)(added_obj + cJSON_AddItemToObject(root, "to_replace", old_node));

    // step 4: Operate - retrieve the array via object lookup, replace an array item via pointer and replace root child via pointer
    obj_values = cJSON_GetObjectItem(root, "values");
    new_num = cJSON_CreateNumber(9.99);
    replaced_arr = cJSON_ReplaceItemViaPointer(arr, num1, new_num);
    replacement_str = cJSON_CreateString("was replaced");
    replaced_obj = cJSON_ReplaceItemViaPointer(root, old_node, replacement_str);

    // step 5: Validate - print and derive a small validation score from results
    printed = cJSON_PrintUnformatted(root);
    validation_score = (int)(printed != (char *)0) + (int)cJSON_IsNumber(cJSON_GetArrayItem(arr, 1)) + (int)cJSON_IsString(cJSON_GetObjectItem(root, "to_replace")) + (int)(obj_values != (cJSON *)0);

    // step 6: Cleanup - free printed buffer and delete the whole JSON tree
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)added_arr;
    (void)added_obj;
    (void)replaced_arr;
    (void)replaced_obj;
    (void)obj_values;
    (void)num0;
    (void)num1;
    (void)new_num;
    (void)old_node;
    (void)replacement_str;
    // API sequence test completed successfully
    return 66;
}