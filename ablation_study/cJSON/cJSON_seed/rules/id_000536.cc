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
//<ID> 536
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json = "{\"person\":{\"name\":\"Alice\",\"age\":30},\"status\":\"ok\"}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, strlen(json) + 1, &parse_end, 1);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);

    // step 2: Setup / Configure
    cJSON *person = cJSON_GetObjectItemCaseSensitive(root, "person");
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(person, "name");
    const char *name_str = cJSON_GetStringValue(name_item);

    // step 3: Operate / Validate
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    size_t to_copy_printed = printed_len > 128 ? 128 : printed_len;
    memcpy(buffer, printed, to_copy_printed);
    size_t name_len = strlen(name_str);
    size_t remaining = 256 - to_copy_printed - 1;
    size_t to_copy_name = name_len > remaining ? remaining : name_len;
    memcpy(buffer + to_copy_printed, name_str, to_copy_name);
    buffer[to_copy_printed + to_copy_name] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}