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
//<ID> 104
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *name = cJSON_CreateString("initial");
    cJSON_AddItemToObject(config, "name", name);
    cJSON_AddNumberToObject(config, "threshold", 42.5);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(config, "enabled", flag);

    // step 2: Configure
    const char *name_str = cJSON_GetStringValue(name);
    cJSON *backup = cJSON_Duplicate(config, 1);
    cJSON_AddItemToObject(root, "backup", backup);

    // step 3: Operate and Validate
    double first_char_code = (double)(unsigned char)name_str[0];
    cJSON_AddNumberToObject(root, "first_char_code", first_char_code);
    cJSON_bool was_false = cJSON_IsFalse(flag);
    cJSON_AddNumberToObject(root, "was_false_flag", (double)was_false);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}