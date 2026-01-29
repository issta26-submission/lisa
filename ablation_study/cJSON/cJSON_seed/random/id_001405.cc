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
//<ID> 1405
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
    cJSON *elem1 = (cJSON *)0;
    cJSON *elem2 = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *ref_child = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    int arr_size = 0;
    cJSON_bool add_ok = 0;
    int validation_score = 0;

    // step 2: Initialize - create root, array, child object and a false boolean
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    elem1 = cJSON_CreateString("alpha");
    elem2 = cJSON_CreateString("beta");
    child = cJSON_CreateObject();
    false_item = cJSON_CreateFalse();

    // step 3: Configure - populate array and attach child object and flag
    cJSON_AddItemToArray(arr, elem1);
    cJSON_AddItemToArray(arr, elem2);
    cJSON_AddItemToObject(root, "items", arr);
    add_ok = cJSON_AddItemToObjectCS(child, "flag", false_item);
    cJSON_AddItemToObject(root, "meta", child);

    // step 4: Operate - create an object reference to the child and add it to root, pick first array element
    ref_child = cJSON_CreateObjectReference(child);
    add_ok = cJSON_AddItemToObjectCS(root, "meta_ref", ref_child);
    first_elem = cJSON_GetArrayItem(arr, 0);

    // step 5: Validate - gather array size and simple checks into a score
    arr_size = cJSON_GetArraySize(arr);
    validation_score = arr_size + (int)(first_elem != (cJSON *)0) + (int)cJSON_IsFalse(false_item) + (int)add_ok;
    (void)validation_score;

    // step 6: Cleanup - delete the root (which owns the whole tree)
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}