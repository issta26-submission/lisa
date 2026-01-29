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
//<ID> 673
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
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON_AddStringToObject(config, "name", "example");
    cJSON_AddNumberToObject(config, "version", 1.0);
    cJSON *sub = cJSON_AddObjectToObject(config, "sub");
    cJSON_AddStringToObject(sub, "desc", "nested object");
    cJSON_AddItemToArray(items, cJSON_CreateString("first"));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(123.0));

    // step 3: Operate and Validate
    cJSON *config_copy = cJSON_Duplicate(config, 1);
    cJSON_AddItemToObject(root, "config_copy", config_copy);
    cJSON *first = cJSON_GetArrayItem(items, 0);
    cJSON_bool first_is_str = cJSON_IsString(first);
    cJSON_AddBoolToObject(root, "first_is_string", first_is_str);
    char *rawbuf = (char *)cJSON_malloc(512);
    memset(rawbuf, 0, 512);
    cJSON_PrintPreallocated(root, rawbuf, 512, 1);
    cJSON_AddStringToObject(root, "snapshot", rawbuf);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(rawbuf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}