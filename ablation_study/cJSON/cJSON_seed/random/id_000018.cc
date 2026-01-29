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
//<ID> 18
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
    cJSON *str_item = cJSON_CreateString("original");
    cJSON *detached = (cJSON *)0;
    cJSON_bool add_ref_result = 0;
    char json_buf[] = " { \"k1\" : \"v1\" , \"k2\": [ 10, 20 ] } ";

    // step 2: Setup
    cJSON_AddItemToArray(arr, str_item);
    add_ref_result = cJSON_AddItemReferenceToObject(root, "shared", str_item);

    // step 3: Operate
    detached = cJSON_DetachItemFromArray(arr, 0);
    cJSON_SetValuestring(detached, "modified");
    cJSON_Minify(json_buf);

    // step 4: Validate
    cJSON_DeleteItemFromObject(root, "shared");

    // step 5: Cleanup
    cJSON_Delete(arr);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}