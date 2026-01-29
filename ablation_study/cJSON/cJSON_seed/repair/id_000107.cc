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
//<ID> 107
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
    cJSON *config = cJSON_AddObjectToObject(root, "config");

    // step 2: Configure
    cJSON *nameItem = cJSON_CreateString("example");
    cJSON_AddItemToObject(config, "name", nameItem);
    cJSON_AddNumberToObject(config, "version", 1.0);
    cJSON_AddNumberToObject(meta, "created", 1234567890.0);

    // step 3: Operate and Validate
    char *namevalue = cJSON_GetStringValue(nameItem);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", flag);
    cJSON_bool flag_is_false = cJSON_IsFalse(flag);
    double flag_num = (double)flag_is_false;
    cJSON_AddNumberToObject(root, "disabled_flag", flag_num);
    double first_char_code = (double)(unsigned char)namevalue[0];
    cJSON_AddNumberToObject(root, "first_char_code", first_char_code);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}