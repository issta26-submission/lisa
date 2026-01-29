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
//<ID> 109
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
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObject(meta, "name", name);
    cJSON *active = cJSON_CreateFalse();
    cJSON_AddItemToObject(meta, "active", active);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "version", 1.2);
    cJSON_AddNumberToObject(meta, "rating", 4.5);

    // step 3: Operate and Validate
    char *name_val = cJSON_GetStringValue(name);
    double first_char_code = (double)(unsigned char)name_val[0];
    cJSON_AddNumberToObject(root, "first_char", first_char_code);
    double inactive_flag = (double)cJSON_IsFalse(active);
    cJSON_AddNumberToObject(root, "inactive_flag", inactive_flag);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}