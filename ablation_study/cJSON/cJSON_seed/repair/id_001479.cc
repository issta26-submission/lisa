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
//<ID> 1479
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
    cJSON *name = cJSON_CreateString("tester");
    cJSON_AddItemToObject(meta, "name", name);
    cJSON *flag_true = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag_on", flag_true);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag_off", flag_false);

    // step 2: Configure
    cJSON *counter = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(root, "counter", counter);
    double new_count = cJSON_SetNumberHelper(counter, 123.456);

    // step 3: Operate and Validate
    const char *name_str = cJSON_GetStringValue(name);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = name_str ? name_str[0] : 'x';
    scratch[1] = (char)('0' + ((int)new_count % 10));
    scratch[2] = out_len ? out[0] : ' ';
    scratch[3] = (char)('0' + (int)cJSON_IsTrue(flag_true));
    scratch[4] = (char)('0' + (int)cJSON_IsFalse(flag_false));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}