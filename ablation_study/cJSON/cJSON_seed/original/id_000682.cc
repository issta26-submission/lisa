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
//<ID> 682
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"name\":\"original\",\"count\":3,\"info\":{\"desc\":\"sample\"}}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json, &parse_end, 1);
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    const char *name_val = cJSON_GetStringValue(name_item);

    // step 2: Configure
    cJSON *alias_item = cJSON_CreateString("placeholder");
    cJSON_AddItemToObject(root, "alias", alias_item);
    cJSON_SetValuestring(alias_item, name_val);

    // step 3: Operate & Validate
    const char *alias_val = cJSON_GetStringValue(alias_item);
    (void)alias_val;
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}