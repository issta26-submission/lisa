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
//<ID> 56
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char json[] = "{\"name\":\"original\",\"value\":42}";
    const char *parse_end = NULL;
    size_t buffer_length = sizeof(json) - 1;
    cJSON *root = NULL;
    cJSON *name_item = NULL;
    cJSON *raw_item = NULL;
    cJSON *added_raw = NULL;
    cJSON *retrieved = NULL;

    // step 2: Setup (parse JSON into cJSON structure)
    root = cJSON_ParseWithLengthOpts(json, buffer_length, &parse_end, 1);

    // step 3: Operate / Validate (create raw items, add to object, replace existing item, retrieve)
    name_item = cJSON_GetObjectItemCaseSensitive(root, "name");
    raw_item = cJSON_CreateRaw("\"replaced_raw_value\"");
    added_raw = cJSON_CreateRaw("{\"internal\":true}");
    cJSON_AddItemToObject(root, "added_raw", added_raw);
    cJSON_ReplaceItemViaPointer(root, name_item, raw_item);
    retrieved = cJSON_GetObjectItemCaseSensitive(root, "added_raw");

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}