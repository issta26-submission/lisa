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
//<ID> 36
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
    cJSON *item_one = cJSON_CreateString("alpha");
    cJSON *item_two = cJSON_CreateString("beta");
    cJSON *bool_item = cJSON_CreateBool(0);
    cJSON *detached_item = (cJSON *)0;

    // step 2: Setup
    cJSON_AddItemToArray(root_array, item_one);
    cJSON_AddItemToArray(root_array, item_two);
    cJSON_AddItemToArray(root_array, bool_item);

    // step 3: Configure
    cJSON_AddItemToObjectCS(root_obj, "arr", root_array);
    cJSON_AddTrueToObject(root_obj, "always_true");

    // step 4: Operate
    detached_item = cJSON_DetachItemFromArray(root_array, 1);
    cJSON_AddItemToObjectCS(root_obj, "moved", detached_item);

    // step 5: Validate
    cJSON *transferred_bool = cJSON_CreateBool(1);
    cJSON_AddItemToObjectCS(root_obj, "transferred_bool", transferred_bool);

    // step 6: Cleanup
    cJSON_Delete(root_obj);

    // API sequence test completed successfully
    return 66;
}