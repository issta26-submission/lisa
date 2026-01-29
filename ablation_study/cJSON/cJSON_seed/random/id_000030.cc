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
//<ID> 30
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *bool_item = cJSON_CreateBool(0);
    cJSON *num_item = cJSON_CreateNumber(123.0);
    cJSON *detached_item = (cJSON *)0;
    cJSON_bool add_ok = 0;
    cJSON *found = (cJSON *)0;

    // step 2: Setup
    cJSON_AddItemToArray(arr, bool_item);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddTrueToObject(root, "always_true");

    // step 3: Core operations
    detached_item = cJSON_DetachItemFromArray(arr, 0);
    add_ok = cJSON_AddItemToObjectCS(root, "moved_bool", detached_item);

    // step 4: Validate
    found = cJSON_GetObjectItemCaseSensitive(root, "moved_bool");
    (void)found;
    (void)add_ok;

    // step 5: Cleanup
    cJSON_Delete(arr);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}