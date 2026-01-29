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
//<ID> 190
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json_text[] = "{\"name\":\"source\",\"nested\":{\"key\":\"value\"}}";
    size_t json_len = sizeof(json_text) - 1;
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(json_text, json_len, &parse_end, 1);
    cJSON *root = cJSON_CreateObject();
    cJSON *nested = NULL;
    cJSON *name_item = NULL;
    const char *name_value = NULL;
    cJSON *nested_copy = NULL;
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));

    // step 2: Setup / Configure
    nested = cJSON_GetObjectItemCaseSensitive(parsed, "nested");
    name_item = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    name_value = cJSON_GetStringValue(name_item);
    cJSON_AddStringToObject(root, "imported_name", name_value);
    nested_copy = cJSON_Duplicate(nested, 1);
    cJSON_AddItemToObject(root, "embedded", nested_copy);

    // step 3: Operate / Validate
    cJSON *got_name = cJSON_GetObjectItemCaseSensitive(root, "imported_name");
    const char *got_name_str = cJSON_GetStringValue(got_name);
    cJSON *got_embedded = cJSON_GetObjectItemCaseSensitive(root, "embedded");
    cJSON *got_key = cJSON_GetObjectItemCaseSensitive(got_embedded, "key");
    const char *got_key_str = cJSON_GetStringValue(got_key);
    buffer[0] = got_name_str ? got_name_str[0] : '\0';
    buffer[1] = got_key_str ? got_key_str[0] : '\0';
    (void)parse_end;
    (void)got_name_str;
    (void)got_key_str;

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}