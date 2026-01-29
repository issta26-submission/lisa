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
//<ID> 289
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *array_owner = (cJSON *)0;
    cJSON *array_ref = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *detached_item = (cJSON *)0;
    char json_buf[] = "  [ false ,  true ]  ";
    cJSON_bool add_ref_ok = (cJSON_bool)0;
    cJSON_bool is_false = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    array_owner = cJSON_CreateArray();
    array_ref = cJSON_CreateArray();
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(array_owner, false_item);

    // step 3: Configure
    add_ref_ok = cJSON_AddItemReferenceToArray(array_ref, false_item);
    detached_item = cJSON_DetachItemFromArray(array_owner, 0);
    cJSON_Minify(json_buf);

    // step 4: Operate
    is_false = cJSON_IsFalse(detached_item);
    cJSON_AddItemToObject(root, "owner_array", array_owner);
    cJSON_AddItemToObject(root, "ref_array", array_ref);

    // step 5: Validate
    cJSON_AddBoolToObject(root, "detached_was_false", is_false);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}