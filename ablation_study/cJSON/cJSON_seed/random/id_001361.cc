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
//<ID> 1361
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
    cJSON *str1 = (cJSON *)0;
    cJSON *str2 = (cJSON *)0;
    cJSON *ref_to_arr = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *elem0 = (cJSON *)0;
    char *printed = (char *)0;
    const char *elem0_str = (const char *)0;
    int size = 0;
    cJSON_bool is_arr = 0;
    cJSON_bool is_str = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object, an array and two string items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str1 = cJSON_CreateString("alpha");
    str2 = cJSON_CreateString("beta");

    // step 3: Configure - attach strings to array and array to root; add an object reference to the same array
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToArray(arr, str2);
    cJSON_AddItemToObject(root, "letters", arr);

    // Use AddItemReferenceToObject to add a reference to the existing array into root.
    // This avoids creating another object that would lead to double-free issues on deletion.
    cJSON_AddItemReferenceToObject(root, "letters_ref", arr);
    ref_to_arr = cJSON_GetObjectItem(root, "letters_ref");

    // step 4: Operate - retrieve array, inspect size and types, and produce unformatted JSON
    retrieved_arr = cJSON_GetObjectItem(root, "letters");
    size = cJSON_GetArraySize(retrieved_arr);
    is_arr = cJSON_IsArray(retrieved_arr);
    elem0 = cJSON_GetArrayItem(retrieved_arr, 0);
    is_str = cJSON_IsString(elem0);
    elem0_str = cJSON_GetStringValue(elem0);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate - aggregate basic checks into a score (no branching)
    validation_score = (int)(root != (cJSON *)0) + (int)(retrieved_arr != (cJSON *)0) + (int)(size == 2) + (int)(is_arr != 0) + (int)(is_str != 0) + (int)(elem0_str != (const char *)0) + (int)(printed != (char *)0);

    // step 6: Cleanup - free printed string and delete the root (which frees attached items)
    if (printed != (char *)0) { cJSON_free(printed); printed = (char *)0; }
    cJSON_Delete(root);
    (void)validation_score;
    (void)elem0;
    (void)elem0_str;
    (void)ref_to_arr;
    // API sequence test completed successfully
    return 66;
}