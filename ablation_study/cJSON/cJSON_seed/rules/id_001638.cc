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
//<ID> 1638
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *child = NULL;
    cJSON *greeting = NULL;
    cJSON *sub = NULL;
    cJSON *null_item = NULL;
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool has_child = 0;
    cJSON_bool has_ref = 0;
    cJSON_bool printed = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    child = cJSON_AddObjectToObject(root, "child");
    greeting = cJSON_CreateString("hello world");
    cJSON_AddItemToObject(root, "greeting", greeting);
    sub = cJSON_AddObjectToObject(child, "sub");
    cJSON_AddNumberToObject(sub, "value", 42.0);
    null_item = cJSON_AddNullToObject(child, "maybe");
    cJSON_AddItemReferenceToObject(child, "greeting_ref", greeting);

    // step 3: Operate / Validate
    has_child = cJSON_HasObjectItem(root, "child");
    has_ref = cJSON_HasObjectItem(child, "greeting_ref");
    printed = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}