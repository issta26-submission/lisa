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
//<ID> 539
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    const char *json = "{\"person\":{\"name\":\"Alice\",\"age\":30},\"status\":\"ok\"}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, strlen(json) + 1, &parse_end, 1);

    // step 2: Setup / Configure
    cJSON *person = cJSON_GetObjectItemCaseSensitive(root, "person");
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(person, "name");
    char *name_str = cJSON_GetStringValue(name_item);
    cJSON *status_item = cJSON_GetObjectItemCaseSensitive(root, "status");
    char *status_str = cJSON_GetStringValue(status_item);
    cJSON *greeting = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greeting", greeting);

    // step 3: Operate / Validate
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    size_t copy_len = printed_len > 252 ? 252 : printed_len;
    buffer[0] = name_str ? name_str[0] : '\0';
    buffer[1] = ',';
    buffer[2] = status_str ? status_str[0] : '\0';
    buffer[3] = '\0';
    memcpy(buffer + 4, printed, copy_len);
    buffer[4 + copy_len] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}