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
//<ID> 1368
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
    cJSON *ref_child = (cJSON *)0; /* kept for compatibility, not used to avoid double-free issues */
    cJSON *found_arr = (cJSON *)0;
    cJSON *found_child = (cJSON *)0;
    cJSON *found_greet = (cJSON *)0;
    char *unformatted = (char *)0;
    int arr_size = 0;
    int is_array_flag = 0;
    int is_string_flag = 0;
    int validation_score = 0;

    // step 2: Initialize - create root, an array with two strings, and a child object
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s1 = cJSON_CreateString("alpha");
    s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "letters", arr);
    child_obj = cJSON_CreateObject();
    cJSON_AddStringToObject(child_obj, "greet", "hello");
    cJSON_AddItemToObject(root, "child", child_obj);

    // step 3: Configure - add a reference to the existing child (use AddItemReference to avoid creating an owning duplicate)
    /* ref_child = cJSON_CreateObjectReference(child_obj); */
    cJSON_AddItemReferenceToObject(root, "child_ref", child_obj);

    // step 4: Operate - inspect the array and string node, and produce unformatted JSON
    found_arr = cJSON_GetObjectItem(root, "letters");
    is_array_flag = (int)cJSON_IsArray(found_arr);
    arr_size = cJSON_GetArraySize(found_arr);
    found_child = cJSON_GetObjectItem(root, "child");
    found_greet = cJSON_GetObjectItem(found_child, "greet");
    is_string_flag = (int)cJSON_IsString(found_greet);
    unformatted = cJSON_PrintUnformatted(root);

    // step 5: Validate - compose a simple validation score and cleanup resources
    validation_score = (int)(root != (cJSON *)0) + is_array_flag + (int)(arr_size == 2) + is_string_flag + (int)(unformatted != (char *)0);
    if (unformatted) {
        cJSON_free(unformatted);
    }
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)found_arr;
    (void)found_child;
    (void)found_greet;
    return 66;
}