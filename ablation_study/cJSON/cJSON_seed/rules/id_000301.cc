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
//<ID> 301
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *flag = cJSON_CreateTrue();
    cJSON *greeting = cJSON_CreateString("hello world");
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON_bool flag_is_bool = 0;
    const char *greet_val = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON_AddItemToObject(root, "greeting", greeting);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddTrueToObject(meta, "subflag");
    cJSON_AddNumberToObject(meta, "count", 7);

    // step 3: Operate / Validate
    cJSON *retrieved_flag = cJSON_GetObjectItem(root, "flag");
    flag_is_bool = cJSON_IsBool(retrieved_flag);
    cJSON *retrieved_greeting = cJSON_GetObjectItem(root, "greeting");
    greet_val = cJSON_GetStringValue(retrieved_greeting);
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = (char)('0' + (int)flag_is_bool);
    buffer[1] = greet_val[0];
    buffer[2] = printed[0];
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}