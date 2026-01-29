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
//<ID> 199
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json_text = "{\"greeting\":\"hello\",\"nested\":{\"key\":\"value\"}}";
    size_t json_len = strlen(json_text);
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(json_text, json_len, &parse_end, 1);
    cJSON *root = cJSON_CreateObject();
    const char *greeting_text = NULL;
    const char *status_text = NULL;
    cJSON *status_str = NULL;
    cJSON *parsed_node = NULL;
    cJSON *greeting_node = NULL;
    cJSON *status_node = NULL;

    // step 2: Setup / Configure
    status_str = cJSON_AddStringToObject(root, "status", "ok");
    cJSON_AddItemToObject(root, "parsed", parsed);

    // step 3: Operate / Validate
    parsed_node = cJSON_GetObjectItemCaseSensitive(root, "parsed");
    greeting_node = cJSON_GetObjectItemCaseSensitive(parsed_node, "greeting");
    status_node = cJSON_GetObjectItemCaseSensitive(root, "status");
    greeting_text = cJSON_GetStringValue(greeting_node);
    status_text = cJSON_GetStringValue(status_node);
    (void)greeting_text;
    (void)status_text;
    (void)status_str;
    (void)parse_end;

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}