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
//<ID> 535
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
    const char *json = "{\"user\":{\"name\":\"alice\",\"age\":30},\"status\":\"ok\"}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, strlen(json) + 1, &parse_end, 1);
    cJSON *user = cJSON_GetObjectItemCaseSensitive(root, "user");
    cJSON *name = cJSON_GetObjectItemCaseSensitive(user, "name");

    // step 2: Setup / Configure
    char *name_str = cJSON_GetStringValue(name);
    size_t name_len = strlen(name_str);
    size_t copy_name_len = name_len < 64 ? name_len : 64;
    memcpy(buffer, name_str, copy_name_len);
    cJSON *info = cJSON_CreateObject();
    cJSON_AddStringToObject(info, "extracted_name", name_str);
    cJSON_AddItemToObject(root, "info", info);

    // step 3: Operate / Validate
    cJSON *status = cJSON_GetObjectItemCaseSensitive(root, "status");
    char *status_str = cJSON_GetStringValue(status);
    size_t status_len = strlen(status_str);
    size_t copy_status_len = status_len < 64 ? status_len : 64;
    memcpy(buffer + 128, status_str, copy_status_len);
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    size_t copy_print_len = printed_len < 256 ? printed_len : 256;
    memcpy(buffer + 64, printed, copy_print_len);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}