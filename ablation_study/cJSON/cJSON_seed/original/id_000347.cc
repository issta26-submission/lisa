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
//<ID> 347
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"numbers\": [1, 2, 3], \"meta\": {\"name\": \"example\"}}";
    cJSON *root = cJSON_Parse(json);

    // step 2: Configure
    cJSON *numbers = cJSON_GetObjectItemCaseSensitive(root, "numbers");
    cJSON *meta = cJSON_GetObjectItemCaseSensitive(root, "meta");
    cJSON *name = cJSON_GetObjectItemCaseSensitive(meta, "name");
    const char *name_str = cJSON_GetStringValue(name);
    cJSON *refs = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "refs_container", refs);
    cJSON_AddItemReferenceToArray(refs, numbers);
    cJSON_AddStringToObject(root, "name_copy", name_str);

    // step 3: Operate and Validate
    char *printed = cJSON_Print(root);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}