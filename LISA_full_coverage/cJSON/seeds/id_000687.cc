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
//<ID> 687
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"name\":\"original\",\"value\":123}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json, &parse_end, 1);
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    char *orig_name = cJSON_GetStringValue(name_item);

    // step 2: Configure
    cJSON *mirror = cJSON_CreateString("placeholder");
    cJSON_SetValuestring(mirror, orig_name);
    cJSON_AddItemToObject(root, "mirror", mirror);
    cJSON_SetValuestring(name_item, "updated");

    // step 3: Operate & Validate
    char *current_name = cJSON_GetStringValue(name_item);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}