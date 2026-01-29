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
//<ID> 14
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
    cJSON *array_item = cJSON_CreateString("array_value");
    cJSON *detached_item = (cJSON *)0;
    char json_buf[] = "{\"alpha\":\"beta\"}";
    cJSON *parsed = (cJSON *)0;
    cJSON *parsed_child = (cJSON *)0;

    // step 2: Setup
    cJSON_AddItemToArray(root_array, array_item);
    detached_item = cJSON_DetachItemFromArray(root_array, 0);
    cJSON_AddItemReferenceToObject(root_obj, "myref", detached_item);

    // step 3: Configure
    cJSON_Minify(json_buf);
    parsed = cJSON_Parse(json_buf);
    parsed_child = cJSON_GetObjectItem(parsed, "alpha");

    // step 4: Operate
    cJSON_SetValuestring(parsed_child, "newbeta");

    // step 5: Validate
    cJSON_DeleteItemFromObject(root_obj, "myref");

    // step 6: Cleanup
    cJSON_Delete(detached_item);
    cJSON_Delete(parsed);
    cJSON_Delete(root_array);
    cJSON_Delete(root_obj);

    // API sequence test completed successfully
    return 66;
}