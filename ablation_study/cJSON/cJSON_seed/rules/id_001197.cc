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
//<ID> 1197
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *greeting = NULL;
    cJSON *flag = NULL;
    const char *greeting_text = NULL;
    char *printed = NULL;
    cJSON_bool has_greeting = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    greeting = cJSON_CreateString("hello world");
    cJSON_AddItemToObject(root, "greeting", greeting);
    flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", flag);

    // step 3: Operate / Validate
    has_greeting = cJSON_HasObjectItem(root, "greeting");
    greeting_text = cJSON_GetStringValue(cJSON_GetObjectItem(root, "greeting"));
    printed = cJSON_Print(root);
    (void)has_greeting;
    (void)greeting_text;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}