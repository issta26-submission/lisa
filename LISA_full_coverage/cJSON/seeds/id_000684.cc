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
//<ID> 684
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"name\":\"original\",\"count\":3}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json, &parse_end, 1);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    char *orig_name = cJSON_GetStringValue(name_item);
    cJSON_AddItemToObject(root, "original_name", cJSON_CreateString(orig_name));

    // step 3: Operate
    char *setret = cJSON_SetValuestring(name_item, "modified_name");
    cJSON *copied_name_item = cJSON_CreateString(setret);
    cJSON_AddItemToObject(root, "copy_of_name", copied_name_item);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    char *current_name = cJSON_GetStringValue(name_item);
    (void)current_name;
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}