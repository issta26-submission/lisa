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
//<ID> 792
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("device");
    cJSON *model = cJSON_CreateString("X1000");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "name", name);
    cJSON_AddItemToObject(meta, "model", model);

    // step 2: Configure
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);

    // step 3: Operate & Validate
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_meta_cs = cJSON_GetObjectItemCaseSensitive(parsed, "meta");
    cJSON *parsed_name = cJSON_GetObjectItem(parsed_meta_cs, "name");
    char *extracted_name = cJSON_GetStringValue(parsed_name);
    cJSON *copied_name = cJSON_CreateString(extracted_name);
    cJSON_AddItemToObject(parsed, "name_copy", copied_name);
    cJSON *verify_item = cJSON_GetObjectItem(parsed, "name_copy");
    char *verify_value = cJSON_GetStringValue(verify_item);
    (void)verify_value;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}