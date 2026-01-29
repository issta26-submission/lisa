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
//<ID> 1363
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
    cJSON *ref_s1 = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool is_array_flag = 0;
    cJSON_bool is_str_flag = 0;
    int size = 0;
    int validation_score = 0;

    // step 2: Initialize - create root, array and two string items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s1 = cJSON_CreateString("alpha");
    s2 = cJSON_CreateString("beta");

    // step 3: Configure - populate array and attach to root, create an object reference to s1 and add it
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "letters", arr);
    ref_s1 = cJSON_CreateObjectReference(s1);
    cJSON_AddItemToObject(root, "alpha_ref", ref_s1);

    // step 4: Operate - retrieve the array, inspect type and size, inspect first element, and print unformatted
    retrieved_arr = cJSON_GetObjectItem(root, "letters");
    is_array_flag = cJSON_IsArray(retrieved_arr);
    size = cJSON_GetArraySize(retrieved_arr);
    first_elem = cJSON_GetArrayItem(retrieved_arr, 0);
    is_str_flag = cJSON_IsString(first_elem);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate - aggregate simple checks into a score
    validation_score = (int)(root != (cJSON *)0) + (int)(retrieved_arr != (cJSON *)0) + (int)is_array_flag + (int)is_str_flag + (int)(size == 2) + (int)(printed != (char *)0);

    // step 6: Cleanup - free printed buffer and delete root which frees attached items
    if (printed) { cJSON_free(printed); } /* single conditional used only for safe free */
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)first_elem;
    (void)ref_s1;
    return 66;
}