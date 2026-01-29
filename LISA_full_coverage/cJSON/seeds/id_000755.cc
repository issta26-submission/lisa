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
//<ID> 755
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"name\":\"alpha\",\"data\":{\"key\":\"value\"}}";
    cJSON *root = cJSON_Parse(json_text);

    // step 2: Configure
    cJSON *raw_item = cJSON_CreateRaw("{\"inserted\":true}");
    cJSON_AddItemToObject(root, "raw", raw_item);
    cJSON *name_item = cJSON_DetachItemFromObject(root, "name");
    cJSON_SetValuestring(name_item, "beta");
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "raw");

    // step 3: Operate
    char *buffered = cJSON_PrintBuffered(root, 256, 1);

    // step 4: Validate & Cleanup
    cJSON_free(buffered);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}