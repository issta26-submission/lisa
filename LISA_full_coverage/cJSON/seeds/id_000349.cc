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
//<ID> 349
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"data\":[1,2,3],\"meta\":{\"name\":\"example\"}}";
    cJSON *root = cJSON_Parse(json_text);

    // step 2: Configure
    cJSON *data_arr = cJSON_GetObjectItemCaseSensitive(root, "data");
    cJSON *first_item = cJSON_GetArrayItem(data_arr, 0);
    cJSON *refs = cJSON_CreateArray();
    cJSON_AddItemReferenceToArray(refs, first_item);
    cJSON_AddItemToObject(root, "refs", refs);

    // step 3: Operate and Validate
    char *printed = cJSON_Print(root);
    int data_count = cJSON_GetArraySize(data_arr);
    int refs_count = cJSON_GetArraySize(refs);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}