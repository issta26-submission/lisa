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
//<ID> 39
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
    cJSON *bool_true = cJSON_CreateBool(1);
    cJSON *bool_false = cJSON_CreateBool(0);
    cJSON *detached_item = (cJSON *)0;

    // step 2: Setup - populate array and attach to root
    cJSON_AddItemToArray(arr, bool_true);
    cJSON_AddItemToArray(arr, bool_false);
    cJSON_AddItemToObjectCS(root, "items", arr);

    // step 3: Operate - detach the first element from the array
    detached_item = cJSON_DetachItemFromArray(arr, 0);

    // step 4: Configure/Integrate - add detached item into root and add a true flag
    cJSON_AddItemToObjectCS(root, "moved_item", detached_item);
    cJSON_AddTrueToObject(root, "always_true");

    // step 5: Cleanup - free all allocated cJSON structures
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}