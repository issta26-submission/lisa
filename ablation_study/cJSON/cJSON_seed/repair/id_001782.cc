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
//<ID> 1782
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *ver_item = cJSON_AddNumberToObject(config, "version", 3.14);
    cJSON_AddStringToObject(config, "env", "production");
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "author", "tester");
    cJSON_AddNumberToObject(root, "threshold", 100.0);

    // step 2: Configure
    cJSON *notes = cJSON_CreateString("initialization complete");
    cJSON_AddItemToObject(root, "notes", notes);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);

    // step 3: Operate and Validate
    const char *author_str = cJSON_GetStringValue(cJSON_GetObjectItem(meta, "author"));
    const char *env_str = cJSON_GetStringValue(cJSON_GetObjectItem(config, "env"));
    double version_val = cJSON_GetNumberValue(ver_item);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = author_str ? author_str[0] : 'Y';
    buffer[2] = env_str ? env_str[0] : 'Z';
    buffer[3] = (char)('0' + ((int)version_val % 10));
    cJSON_DeleteItemFromObject(root, "notes");

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}