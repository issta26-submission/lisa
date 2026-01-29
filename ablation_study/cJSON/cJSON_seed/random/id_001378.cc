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
//<ID> 1378
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
    cJSON *s1 = (cJSON *)0;
    cJSON *s2 = (cJSON *)0;
    cJSON *found_arr = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    cJSON *greet_item = (cJSON *)0;
    char *rendered = (char *)0;
    int is_array_flag = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object, an array and two string elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s1 = cJSON_CreateString("one");
    s2 = cJSON_CreateString("two");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);

    // step 3: Configure - attach the array and add a direct string to the root
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddStringToObject(root, "greeting", "hello");

    // step 4: Operate - retrieve the array and its first element, and the greeting
    found_arr = cJSON_GetObjectItem(root, "numbers");
    is_array_flag = (int)cJSON_IsArray(found_arr);
    first_elem = cJSON_GetArrayItem(found_arr, 0);
    greet_item = cJSON_GetObjectItem(root, "greeting");

    // step 5: Render and minify, then compute a simple validation score
    rendered = cJSON_Print(root);
    cJSON_Minify(rendered);
    validation_score = (int)(root != (cJSON *)0) + is_array_flag + (int)(first_elem != (cJSON *)0) + (int)(greet_item != (cJSON *)0) + (int)(rendered != (char *)0);

    // step 6: Cleanup - free rendered string and delete the JSON tree
    cJSON_free(rendered);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)first_elem;
    (void)greet_item;
    return 66;
}