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
//<ID> 1592
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON *extra_num = cJSON_CreateNumber(3.5);
    cJSON_AddItemToObject(root, "extra", extra_num);
    cJSON *name = cJSON_CreateString("device-alpha");
    cJSON_AddItemReferenceToObject(settings, "name_ref", name);
    cJSON_AddNumberToObject(settings, "version", 2.1);

    // step 2: Configure
    cJSON *ver_item = cJSON_GetObjectItemCaseSensitive(settings, "version");
    double ver_val = cJSON_GetNumberValue(ver_item);
    cJSON *scaled = cJSON_CreateNumber(ver_val * 10.0);
    cJSON_AddItemToObject(settings, "version_scaled", scaled);
    cJSON *name_copy = cJSON_CreateString("device-copy");
    cJSON_AddItemToObject(root, "name_copy", name_copy);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    int ver_int = (int)ver_val;
    scratch[0] = (char)('0' + (ver_int % 10));
    scratch[1] = out_len ? out[0] : ' ';
    scratch[2] = (char)('A');

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}