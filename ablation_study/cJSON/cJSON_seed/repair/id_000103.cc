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
//<ID> 103
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_AddObjectToObject(root, "settings");
    cJSON *info = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "info", info);
    cJSON *enabled = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", enabled);

    // step 2: Configure
    const char *s = cJSON_GetStringValue(info);
    cJSON_bool is_false = cJSON_IsFalse(enabled);
    double first_char = (double)(unsigned char)s[0];
    double computed = first_char * (double)is_false + 3.14;
    cJSON_AddNumberToObject(root, "computed", computed);

    // step 3: Operate and Validate
    cJSON *inner = cJSON_AddObjectToObject(settings, "inner");
    cJSON_AddNumberToObject(inner, "adjusted", computed + 2.0);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}