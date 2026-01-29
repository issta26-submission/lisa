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
//<ID> 1472
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *cfg = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "cfg", cfg);
    cJSON *name = cJSON_CreateString("example-name");
    cJSON_AddItemToObject(cfg, "name", name);
    cJSON *num = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(cfg, "threshold", num);
    cJSON *flag_true = cJSON_CreateTrue();
    cJSON_AddItemToObject(cfg, "active", flag_true);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(cfg, "deprecated", flag_false);

    // step 2: Configure
    double updated = cJSON_SetNumberHelper(num, 42.5);
    char *name_val = cJSON_GetStringValue(name);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = name_val ? name_val[0] : 'n';
    scratch[1] = (char)('0' + ((int)updated % 10));
    scratch[2] = out_len ? out[0] : ' ';
    scratch[3] = 'V';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}