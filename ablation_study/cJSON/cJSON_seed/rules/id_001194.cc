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
//<ID> 1194
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
    cJSON *greeting_copy = NULL;
    char *printed = NULL;
    const char *greeting_text = NULL;
    cJSON_bool has_greeting = 0;
    cJSON_bool has_copy = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    greeting = cJSON_CreateString("hello world");
    cJSON_AddItemToObject(root, "greeting", greeting);
    cJSON_AddItemToObject(root, "enabled", cJSON_CreateFalse());

    // step 3: Operate / Validate
    has_greeting = cJSON_HasObjectItem(root, "greeting");
    greeting_text = cJSON_GetStringValue(cJSON_GetObjectItem(root, "greeting"));
    greeting_copy = cJSON_CreateString(greeting_text);
    cJSON_AddItemToObject(root, "greeting_copy", greeting_copy);
    has_copy = cJSON_HasObjectItem(root, "greeting_copy");
    printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}