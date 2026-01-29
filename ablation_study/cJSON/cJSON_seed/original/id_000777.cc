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
//<ID> 777
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char json_text[] = "{\"items\":[{\"name\":\"one\"},{\"name\":\"two\"}],\"meta\":{\"count\":2}}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json_text, (size_t)(sizeof(json_text) - 1), &parse_end, 1);

    // step 2: Configure
    cJSON *items = cJSON_GetObjectItemCaseSensitive(root, "items");
    cJSON_AddNullToObject(root, "maybeNull");
    cJSON_AddRawToObject(root, "extra_raw", "{\"extra\":true}");

    // step 3: Operate
    cJSON *detached = cJSON_DetachItemFromArray(items, 1);
    cJSON *detached_name_obj = cJSON_GetObjectItemCaseSensitive(detached, "name");
    const char *detached_name = cJSON_GetStringValue(detached_name_obj);

    // step 4: Validate & Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    (void)detached_name;
    cJSON_free(printed);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}