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
//<ID> 83
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *greeting = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greeting", greeting);

    // step 2: Configure
    cJSON *greeting_copy = cJSON_Duplicate(greeting, 1);
    cJSON_AddItemToObject(root, "greeting_copy", greeting_copy);
    const char *gval = cJSON_GetStringValue(greeting);
    size_t gval_len = strlen(gval);
    char *gval_buf = (char *)cJSON_malloc(gval_len + 1);
    memcpy(gval_buf, gval, gval_len + 1);

    // step 3: Operate & Validate
    cJSON_bool has_greeting = cJSON_HasObjectItem(root, "greeting");
    cJSON_bool has_greeting_copy = cJSON_HasObjectItem(root, "greeting_copy");
    cJSON_AddItemToObject(root, "has_greeting", cJSON_CreateNumber((double)has_greeting));
    cJSON_AddItemToObject(root, "has_greeting_copy", cJSON_CreateNumber((double)has_greeting_copy));
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_Minify(printed);

    // step 4: Cleanup
    cJSON_free(gval_buf);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}