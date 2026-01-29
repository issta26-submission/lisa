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
//<ID> 390
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
    cJSON *name1 = (cJSON *)0;
    cJSON *name2 = (cJSON *)0;
    cJSON *ref_str = (cJSON *)0;
    int arr_size = 0;
    cJSON_bool add_ref_result = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure
    name1 = cJSON_CreateString("Alice");
    name2 = cJSON_CreateString("Bob");
    cJSON_AddItemToArray(arr, name1);
    cJSON_AddItemToArray(arr, name2);
    cJSON_AddItemToObject(root, "names", arr);
    cJSON_AddBoolToObject(root, "active", 1);
    ref_str = cJSON_CreateString("alias_value");
    add_ref_result = cJSON_AddItemReferenceToObject(root, "alias", ref_str);

    // step 4: Operate
    arr_size = cJSON_GetArraySize(arr);
    (void)add_ref_result;
    (void)arr_size;

    // step 5: Validate
    (void)root;
    (void)name1;
    (void)name2;
    (void)ref_str;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}