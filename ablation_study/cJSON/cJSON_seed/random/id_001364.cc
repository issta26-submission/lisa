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
//<ID> 1364
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
    cJSON *str0 = (cJSON *)0;
    cJSON *str1 = (cJSON *)0;
    cJSON *obj = (cJSON *)0;
    cJSON *name = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem2 = (cJSON *)0;
    cJSON *ref_obj = (cJSON *)0;
    char *printed = (char *)0;
    int size = 0;
    cJSON_bool is_array = 0;
    cJSON_bool is_string_elem0 = 0;
    int validation_score = 0;

    // step 2: Initialize - create root, array, strings and an object with a name
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str0 = cJSON_CreateString("alpha");
    str1 = cJSON_CreateString("beta");
    obj = cJSON_CreateObject();
    name = cJSON_CreateString("gamma");
    cJSON_AddItemToObject(obj, "name", name);

    // step 3: Configure - assemble array and attach to root
    cJSON_AddItemToArray(arr, str0);
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToArray(arr, obj);
    cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate - retrieve array, inspect contents, print unformatted JSON, create object reference
    retrieved = cJSON_GetObjectItem(root, "items");
    size = cJSON_GetArraySize(retrieved);
    is_array = cJSON_IsArray(retrieved);
    elem0 = cJSON_GetArrayItem(retrieved, 0);
    elem2 = cJSON_GetArrayItem(retrieved, 2);
    is_string_elem0 = cJSON_IsString(elem0);
    printed = cJSON_PrintUnformatted(root);
    ref_obj = cJSON_CreateObjectReference(elem2);
    cJSON_AddItemToObject(root, "ref_obj", ref_obj);

    // step 5: Validate - compute a simple validation score without branching
    validation_score = (int)(root != (cJSON *)0) + (int)(retrieved != (cJSON *)0) + (int)(size == 3) + (int)(is_array != 0) + (int)(is_string_elem0 != 0) + (int)(printed != (char *)0) + (int)(ref_obj != (cJSON *)0);

    // step 6: Cleanup - free printed string and delete root which frees all attached items
    if (printed) { cJSON_free(printed); } /* allowed single-statement conditional only for safe free */
    cJSON_Delete(root);

    /* API sequence test completed successfully */
    (void)validation_score;
    (void)elem0;
    (void)elem2;
    return 66;
}