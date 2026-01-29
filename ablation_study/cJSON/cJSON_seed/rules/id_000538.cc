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
//<ID> 538
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
    const char *json = "{\"user\":{\"name\":\"alice\",\"age\":30},\"status\":\"ok\"}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, strlen(json) + 1, &parse_end, 1);

    // step 2: Setup / Configure
    cJSON *user = cJSON_GetObjectItemCaseSensitive(root, "user");
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(user, "name");
    char *name_str = cJSON_GetStringValue(name_item);
    cJSON *summary = cJSON_CreateObject();
    cJSON_AddStringToObject(summary, "user_name", name_str);

    // step 3: Operate / Validate
    char *printed_root = cJSON_PrintUnformatted(root);
    size_t root_len = strlen(printed_root);
    size_t copy_root = root_len > 100 ? 100 : root_len;
    memcpy(buffer, printed_root, copy_root);
    char *printed_summary = cJSON_PrintUnformatted(summary);
    size_t sum_len = strlen(printed_summary);
    size_t copy_sum = sum_len > (size_t)(256 - copy_root - 1) ? (256 - copy_root - 1) : sum_len;
    memcpy(buffer + copy_root, printed_summary, copy_sum);
    buffer[copy_root + copy_sum] = '\0';

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_free(printed_summary);
    cJSON_free(buffer);
    cJSON_Delete(summary);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}