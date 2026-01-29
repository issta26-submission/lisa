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
//<ID> 338
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON *flag = cJSON_CreateTrue();
    cJSON *txt = cJSON_CreateString("hello_world");
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    const char *version = cJSON_Version();
    cJSON *retrieved_settings = NULL;
    cJSON *retrieved_flag = NULL;
    cJSON *retrieved_txt = NULL;
    cJSON_bool flag_true = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON_AddItemToObject(settings, "enabled", flag);
    cJSON_AddItemToObject(root, "greeting", txt);

    // step 3: Operate / Validate
    retrieved_settings = cJSON_GetObjectItem(root, "settings");
    retrieved_flag = cJSON_GetObjectItem(retrieved_settings, "enabled");
    flag_true = cJSON_IsTrue(retrieved_flag);
    retrieved_txt = cJSON_GetObjectItem(root, "greeting");
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = version[0];
    buffer[1] = (char)('0' + (int)flag_true);
    buffer[2] = retrieved_txt->valuestring[0];
    buffer[3] = printed[0];

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}