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
//<ID> 1371
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
    cJSON *child_obj = (cJSON *)0;
    cJSON *found_arr = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    const char *first_str = (const char *)0;
    char *printed = (char *)0;
    int is_array_flag = 0;
    int validation_score = 0;

    // step 2: Initialize - create root, array and string elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s1 = cJSON_CreateString("one");
    s2 = cJSON_CreateString("two");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);

    // step 3: Configure - attach array and a child object with a string
    cJSON_AddItemToObject(root, "letters", arr);
    child_obj = cJSON_CreateObject();
    cJSON_AddStringToObject(child_obj, "greet", "hello");
    cJSON_AddItemToObject(root, "child", child_obj);

    // step 4: Operate - retrieve the array and its first element, produce JSON and minify
    found_arr = cJSON_GetObjectItem(root, "letters");
    is_array_flag = (int)cJSON_IsArray(found_arr);
    first_elem = cJSON_GetArrayItem(found_arr, 0);
    first_str = cJSON_GetStringValue(first_elem);
    printed = cJSON_Print(root);
    cJSON_Minify(printed);

    // step 5: Validate - compute a simple validation score
    validation_score = (int)(root != (cJSON *)0) + is_array_flag + (int)(first_elem != (cJSON *)0) + (int)(first_str != (const char *)0) + (int)(printed != (char *)0);

    // step 6: Cleanup - free printed JSON and delete the cJSON tree
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)first_str;
    (void)first_elem;
    (void)found_arr;
    return 66;
}