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
//<ID> 675
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObject(config, "name", name);
    cJSON_AddNumberToObject(config, "threshold", 3.14);
    cJSON *tags = cJSON_CreateArray();
    cJSON_AddItemToObject(config, "tags", tags);
    cJSON_AddItemToArray(tags, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(tags, cJSON_CreateString("beta"));

    // step 2: Configure
    cJSON *config_copy = cJSON_Duplicate(config, 1);
    cJSON_AddItemToObject(root, "config_copy", config_copy);

    // step 3: Operate and Validate
    cJSON *first_tag = cJSON_GetArrayItem(tags, 0);
    cJSON_bool first_is_str = cJSON_IsString(first_tag);
    cJSON_AddBoolToObject(root, "first_tag_is_string", first_is_str);
    char *out = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    (void)cJSON_PrintPreallocated(root, buffer, 256, 0);
    cJSON_AddStringToObject(root, "snapshot", buffer);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}