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
//<ID> 1288
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"config\":{\"enabled\":true,\"count\":3},\"items\":[1,2,3]}";
    const char * parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json, &parse_end, 1);

    // step 2: Configure
    cJSON_bool has_config = cJSON_HasObjectItem(root, "config");
    (void)has_config;
    cJSON *config = cJSON_GetObjectItem(root, "config");
    cJSON *enabled = cJSON_GetObjectItem(config, "enabled");
    cJSON *false_item = cJSON_CreateFalse();

    // step 3: Operate
    cJSON_ReplaceItemViaPointer(config, enabled, false_item);
    cJSON_AddItemReferenceToObject(root, "ref_config", config);
    cJSON_bool has_ref = cJSON_HasObjectItem(root, "ref_config");
    (void)has_ref;

    // step 4: Validate & Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}