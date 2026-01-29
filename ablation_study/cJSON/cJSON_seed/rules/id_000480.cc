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
//<ID> 480
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *parsed = NULL;
    char *printed = NULL;
    cJSON *greet_item = NULL;
    cJSON *active_item = NULL;
    const char *greet_str = NULL;
    cJSON_bool is_active = 0;

    // step 2: Setup / Configure
    cJSON_AddStringToObject(meta, "greet", "hello");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(root, "title", "rootTitle");
    cJSON_AddTrueToObject(root, "active");

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse(printed);
    greet_item = cJSON_GetObjectItem(parsed, "meta");
    greet_item = cJSON_GetObjectItem(greet_item, "greet");
    greet_str = cJSON_GetStringValue(greet_item);
    active_item = cJSON_GetObjectItem(parsed, "active");
    is_active = cJSON_IsTrue(active_item);
    buffer[0] = (char)('0' + (int)is_active);
    buffer[1] = (greet_str && greet_str[0]) ? greet_str[0] : '\0';
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    return 66;
    // API sequence test completed successfully
}