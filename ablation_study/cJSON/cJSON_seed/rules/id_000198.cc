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
//<ID> 198
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"name\":\"fuzzer\",\"value\":42}";
    size_t buffer_length = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_CreateObject();
    cJSON *parsed = cJSON_ParseWithLengthOpts(json, buffer_length, &parse_end, 1);
    char *scratch = (char *)cJSON_malloc(64);
    memset(scratch, 0, 64);
    const char *name_str = NULL;
    cJSON *retrieved_parsed = NULL;
    cJSON *name_item = NULL;

    // step 2: Setup / Configure
    cJSON_AddStringToObject(root, "status", "ok");
    cJSON_AddItemToObject(root, "parsed", parsed);

    // step 3: Operate / Validate
    retrieved_parsed = cJSON_GetObjectItemCaseSensitive(root, "parsed");
    name_item = cJSON_GetObjectItemCaseSensitive(retrieved_parsed, "name");
    name_str = cJSON_GetStringValue(name_item);
    scratch[0] = name_str ? name_str[0] : '\0';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)parse_end;
    (void)name_str;
    (void)retrieved_parsed;
    (void)name_item;
    return 66;
}