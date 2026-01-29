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
//<ID> 1190
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *false_item = NULL;
    cJSON *str_item = NULL;
    cJSON *msg_ptr = NULL;
    char *printed = NULL;
    const char *msg_value = NULL;
    cJSON_bool has_ok = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "ok", false_item);
    str_item = cJSON_CreateString("hello world");
    cJSON_AddItemToObject(root, "message", str_item);

    // step 3: Operate / Validate
    has_ok = cJSON_HasObjectItem(root, "ok");
    msg_ptr = cJSON_GetObjectItem(root, "message");
    msg_value = cJSON_GetStringValue(msg_ptr);
    printed = cJSON_Print(root);
    (void)has_ok;
    (void)msg_value;
    (void)msg_ptr;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}