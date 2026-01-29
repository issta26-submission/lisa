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
//<ID> 530
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
    const char *json = "{\"user\":{\"name\":\"alice\",\"id\":42},\"active\":true}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, strlen(json) + 1, &parse_end, 1);

    // step 2: Setup / Configure
    cJSON *user = cJSON_GetObjectItemCaseSensitive(root, "user");
    cJSON *name = cJSON_GetObjectItemCaseSensitive(user, "name");
    char *name_str = cJSON_GetStringValue(name);
    cJSON *summary = cJSON_CreateObject();
    cJSON_AddStringToObject(summary, "extracted_name", name_str);

    // step 3: Operate / Validate
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    size_t copy_len = printed_len > 200 ? 200 : printed_len;
    memcpy(buffer, printed, copy_len);
    size_t name_len = strlen(name_str);
    memcpy(buffer + copy_len, ":", 1);
    memcpy(buffer + copy_len + 1, name_str, name_len);
    buffer[copy_len + 1 + name_len] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(summary);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}