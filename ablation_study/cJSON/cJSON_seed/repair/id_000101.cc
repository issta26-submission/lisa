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
//<ID> 101
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddNumberToObject(root, "version", 1.0);
    cJSON *nameItem = cJSON_AddStringToObject(meta, "name", "example");
    cJSON *info = cJSON_AddObjectToObject(meta, "info");
    cJSON_AddNumberToObject(info, "items", 3.0);

    // step 2: Configure
    cJSON *activeItem = cJSON_AddFalseToObject(root, "active");
    cJSON_AddNumberToObject(meta, "priority", 7.0);

    // step 3: Operate and Validate
    char *name_str = cJSON_GetStringValue(nameItem);
    cJSON_bool is_off = cJSON_IsFalse(activeItem);
    cJSON_AddNumberToObject(root, "first_char_code", (double)(unsigned char)name_str[0]);
    cJSON_AddNumberToObject(root, "active_is_false", (double)is_off);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}