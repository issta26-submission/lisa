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
//<ID> 537
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(512);
    memset(buffer, 0, 512);
    const char *json = "{\"person\":{\"name\":\"Alice\",\"age\":30},\"status\":\"active\"}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, strlen(json) + 1, &parse_end, 1);
    cJSON *person = cJSON_GetObjectItemCaseSensitive(root, "person");
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(person, "name");
    const char *name_str = cJSON_GetStringValue(name_item);

    // step 2: Setup / Configure
    cJSON *status_item = cJSON_GetObjectItemCaseSensitive(root, "status");
    const char *status_str = cJSON_GetStringValue(status_item);
    cJSON *summary = cJSON_CreateObject();
    cJSON_AddStringToObject(summary, "username", name_str);
    cJSON_AddStringToObject(summary, "state", status_str);

    // step 3: Operate / Validate
    char *printed = cJSON_PrintUnformatted(summary);
    size_t printed_len = strlen(printed);
    size_t copy_len = printed_len > 200 ? 200 : printed_len;
    memcpy(buffer, printed, copy_len);
    buffer[copy_len] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(summary);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}