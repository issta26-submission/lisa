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
//<ID> 50
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    const char json[] = "{\"name\":\"example\",\"value\":123}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(json, (size_t)(sizeof(json) - 1), &parse_end, 1);
    cJSON *raw_item = cJSON_CreateRaw("{\"placeholder\":true}");
    cJSON *retrieved = NULL;
    char *printed = NULL;

    // step 2: Setup / Configure structure
    cJSON_AddItemToObject(root, "to_replace", raw_item);

    // step 3: Operate / Validate
    retrieved = cJSON_GetObjectItemCaseSensitive(root, "to_replace");
    cJSON_ReplaceItemViaPointer(root, retrieved, parsed);
    retrieved = cJSON_GetObjectItemCaseSensitive(root, "to_replace");
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}