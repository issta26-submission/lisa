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
//<ID> 12
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
    cJSON *str_item = cJSON_CreateString("initial");
    cJSON *ref_item = cJSON_CreateString("reference");
    cJSON *detached = (cJSON *)0;
    char json_buf[] = " { \"alpha\" : [  10,  20 ], \"beta\" :  \"text \" } ";

    // step 2: Setup
    cJSON_AddItemToObject(root, "myarray", arr);
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemReferenceToObject(root, "myref", ref_item);

    // step 3: Configure
    cJSON_SetValuestring(str_item, "updated");
    cJSON_Minify(json_buf);

    // step 4: Operate
    detached = cJSON_DetachItemFromArray(arr, 0);

    // step 5: Validate
    cJSON_DeleteItemFromObject(root, "myref");

    // step 6: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}