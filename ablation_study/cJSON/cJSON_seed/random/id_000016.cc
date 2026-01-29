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
//<ID> 16
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
    cJSON *str_item = cJSON_CreateString("original");
    cJSON *detached_item = (cJSON *)0;
    char *buffer = (char *)cJSON_malloc(128);
    const char raw_json[] = "  { \"a\" : \"b\" , \"c\" : 123 }   ";
    size_t raw_len = sizeof(raw_json); // includes terminating NUL

    // step 2: Setup
    memset(buffer, 0, 128);
    memcpy(buffer, raw_json, raw_len);
    cJSON_Minify(buffer);

    // step 3: Configure
    cJSON_AddItemToArray(root_array, str_item);
    cJSON_AddItemReferenceToObject(root_obj, "shared", str_item);
    cJSON_SetValuestring(str_item, "updated");

    // step 4: Operate
    detached_item = cJSON_DetachItemFromArray(root_array, 0);

    // step 5: Validate
    cJSON_DeleteItemFromObject(root_obj, "shared");

    // step 6: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(detached_item);
    cJSON_Delete(root_array);
    cJSON_Delete(root_obj);

    // API sequence test completed successfully
    return 66;
}