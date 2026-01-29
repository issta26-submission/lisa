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
//<ID> 13
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
    cJSON *itemA = cJSON_CreateString("alpha");
    cJSON *itemB = cJSON_CreateNumber(123.0);
    cJSON *detached = (cJSON *)0;
    char json_buf[] = " {  \"a\" : 1, \"b\" : 2 } ";
    char *newstr = (char *)0;

    // step 2: Setup
    cJSON_AddItemToArray(arr, itemA);
    cJSON_AddItemReferenceToObject(root, "num_ref", itemB);

    // step 3: Configure
    newstr = cJSON_SetValuestring(itemA, "beta");
    cJSON_Minify(json_buf);

    // step 4: Operate
    detached = cJSON_DetachItemFromArray(arr, 0);

    // step 5: Validate
    cJSON_DeleteItemFromObject(root, "num_ref");

    // step 6: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(arr);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}