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
//<ID> 1408
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
    cJSON *child_obj = (cJSON *)0;
    cJSON *child_str = (cJSON *)0;
    cJSON *flag_false = (cJSON *)0;
    cJSON *found_child = (cJSON *)0;
    cJSON *ref_obj = (cJSON *)0;
    cJSON *id_node = (cJSON *)0;
    double id_value = 0.0;
    int flag_is_false = 0;
    int validation_score = 0;

    // step 2: Initialize - create root, an array, an object child and a string
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    child_obj = cJSON_CreateObject();
    child_str = cJSON_CreateString("element_two");
    flag_false = cJSON_CreateFalse();

    // step 3: Configure - populate the child object and the array, attach to root using Case-Sensitive add
    cJSON_AddNumberToObject(child_obj, "id", 7.0);
    cJSON_AddItemToArray(arr, child_obj);
    cJSON_AddItemToArray(arr, child_str);
    cJSON_AddItemToObjectCS(root, "items", arr);
    cJSON_AddItemToObjectCS(root, "flag", flag_false);

    // step 4: Operate - retrieve first array element and create an object reference to it, attach the reference to root
    found_child = cJSON_GetArrayItem(arr, 0);
    ref_obj = cJSON_CreateObjectReference(found_child);
    cJSON_AddItemToObjectCS(root, "ref_to_first", ref_obj);

    // step 5: Validate - read the id from the referenced child and compute a small validation score
    id_node = cJSON_GetObjectItem(found_child, "id");
    id_value = cJSON_GetNumberValue(id_node);
    flag_is_false = (int)cJSON_IsFalse(flag_false);
    validation_score = (int)id_value + flag_is_false + (int)(ref_obj != (cJSON *)0);

    // step 6: Cleanup - delete the entire tree
    cJSON_Delete(root);

    (void)validation_score;
    (void)id_value;
    (void)id_node;
    (void)found_child;
    (void)child_str;
    (void)child_obj;
    // API sequence test completed successfully
    return 66;
}