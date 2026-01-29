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
//<ID> 740
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
    cJSON *null_item = cJSON_CreateNull();
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON *title_item = cJSON_CreateString("example_title");

    // step 2: Configure
    cJSON_AddItemToObject(meta, "nullable", null_item);
    cJSON_AddItemToObject(meta, "answer", num_item);
    cJSON_AddItemToObject(root, "metadata", meta);
    cJSON_AddItemToObject(root, "title", title_item);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *found_meta = cJSON_GetObjectItem(parsed, "metadata");
    cJSON *detached_meta = cJSON_DetachItemFromObjectCaseSensitive(parsed, "metadata");
    cJSON *detached_nullable = cJSON_GetObjectItem(detached_meta, "nullable");
    cJSON *detached_answer = cJSON_GetObjectItem(detached_meta, "answer");
    double answer_value = cJSON_GetNumberValue(detached_answer);
    (void)found_meta;
    (void)detached_nullable;
    (void)answer_value;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(detached_meta);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}