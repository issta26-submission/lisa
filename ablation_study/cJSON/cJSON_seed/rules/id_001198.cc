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
//<ID> 1198
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *child_str = NULL;
    cJSON *child_false = NULL;
    const char *retrieved = NULL;
    char *printed = NULL;
    cJSON_bool has_key = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    child_str = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greeting", child_str);
    child_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "status", child_false);

    // step 3: Operate / Validate
    retrieved = cJSON_GetStringValue(cJSON_GetObjectItem(root, "greeting"));
    has_key = cJSON_HasObjectItem(root, "status");
    printed = cJSON_Print(root);
    (void)retrieved;
    (void)has_key;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}