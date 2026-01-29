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
//<ID> 533
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
    const char *json = "{\"user\":{\"name\":\"Alice\",\"role\":\"admin\"},\"active\":true}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, strlen(json) + 1, &parse_end, 1);

    // step 2: Setup / Configure
    cJSON *user = cJSON_GetObjectItemCaseSensitive(root, "user");
    cJSON *name = cJSON_GetObjectItemCaseSensitive(user, "name");
    const char *name_str = cJSON_GetStringValue(name);
    cJSON *greeting = cJSON_CreateString(name_str);
    cJSON_AddItemToObject(root, "greeting", greeting);

    // step 3: Operate / Validate
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    size_t copy_len = printed_len > 256 ? 256 : printed_len;
    memcpy(buffer, printed, copy_len);
    buffer[copy_len] = '\0';
    cJSON *retrieved = cJSON_GetObjectItemCaseSensitive(root, "greeting");
    const char *retrieved_str = cJSON_GetStringValue(retrieved);
    (void)retrieved_str;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    return 66; // API sequence test completed successfully
}