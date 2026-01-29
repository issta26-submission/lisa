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
//<ID> 193
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json = "{\"key\":\"value\",\"num\":123}";
    size_t len = strlen(json);
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(json, len, &parse_end, 1);
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();

    // step 2: Setup / Configure
    cJSON *greet_item = cJSON_AddStringToObject(root, "greeting", "hello");
    cJSON_AddItemToObject(root, "data", parsed);
    cJSON_AddStringToObject(child, "inner", "deep");
    cJSON_AddItemToObject(root, "child", child);

    // step 3: Operate / Validate
    cJSON *got_greet = cJSON_GetObjectItemCaseSensitive(root, "greeting");
    cJSON *got_data = cJSON_GetObjectItemCaseSensitive(root, "data");
    cJSON *got_key = cJSON_GetObjectItemCaseSensitive(got_data, "key");
    cJSON *got_child = cJSON_GetObjectItemCaseSensitive(root, "child");
    cJSON *got_inner = cJSON_GetObjectItemCaseSensitive(got_child, "inner");
    void *buffer = cJSON_malloc(16);
    memset(buffer, 0, 16);
    ((char *)buffer)[0] = (got_greet && got_greet->valuestring) ? got_greet->valuestring[0] : 0;
    ((char *)buffer)[1] = (got_key && got_key->valuestring) ? got_key->valuestring[0] : 0;
    ((char *)buffer)[2] = (got_inner && got_inner->valuestring) ? got_inner->valuestring[0] : 0;
    (void)greet_item;
    (void)parse_end;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}