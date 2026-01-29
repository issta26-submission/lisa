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
//<ID> 38
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root_obj = cJSON_CreateObject();
    cJSON *root_array = cJSON_CreateArray();
    cJSON *bool_item_false = cJSON_CreateBool(0);
    cJSON *bool_item_true = cJSON_CreateBool(1);
    cJSON *detached_item = (cJSON *)0;

    // step 2: Setup
    cJSON_AddItemToArray(root_array, bool_item_false);
    cJSON_AddItemToArray(root_array, bool_item_true);
    cJSON_AddItemToObjectCS(root_obj, "items", root_array);
    cJSON_AddTrueToObject(root_obj, "explicit_true");

    // step 3: Core operations
    detached_item = cJSON_DetachItemFromArray(root_array, 0);
    cJSON_AddItemToObjectCS(root_obj, "moved_item", detached_item);

    // step 4: Validate
    /* no branches allowed; validation is implicit by re-parenting detached_item into root_obj */

    // step 5: Cleanup
    cJSON_Delete(root_obj);

    // API sequence test completed successfully
    return 66;
}