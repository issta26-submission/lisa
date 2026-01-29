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
//<ID> 1365
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
    cJSON *ref_str1 = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *elem0 = (cJSON *)0;
    char *printed = (char *)0;
    const char *elem0_text = (const char *)0;
    int size = 0;
    cJSON_bool is_arr = 0;
    cJSON_bool is_str = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object, an array and two string items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str1 = cJSON_CreateString("alpha");
    str2 = cJSON_CreateString("beta");

    // step 3: Configure - attach strings to array, add array to root and create object reference to first string
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToArray(arr, str2);
    cJSON_AddItemToObject(root, "items", arr);
    ref_str1 = cJSON_CreateObjectReference(str1);
    cJSON_AddItemToObject(root, "ref_alpha", ref_str1);

    // step 4: Operate - retrieve the array, inspect its size and types, extract a string and print unformatted JSON
    retrieved_arr = cJSON_GetObjectItem(root, "items");
    size = cJSON_GetArraySize(retrieved_arr);
    is_arr = cJSON_IsArray(retrieved_arr);
    elem0 = cJSON_GetArrayItem(retrieved_arr, 0);
    is_str = cJSON_IsString(elem0);
    elem0_text = cJSON_GetStringValue(elem0);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate - aggregate simple consistency checks into a score
    validation_score = (int)(root != (cJSON *)0) + (int)(retrieved_arr != (cJSON *)0) + (int)(size == 2) + (int)(is_arr != 0) + (int)(is_str != 0) + (int)(elem0_text != (const char *)0) + (int)(printed != (char *)0);

    // step 6: Cleanup - free printed buffer and delete the root (which frees children and references)
    if (printed) { cJSON_free(printed); } /* note: this conditional is only for safe free of possibly NULL pointer */
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)elem0;
    (void)elem0_text;
    return 66;
}