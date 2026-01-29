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
//<ID> 1956
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", list);
    cJSON *item1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(list, item1);
    cJSON *item2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(list, item2);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    const char *version = cJSON_Version();
    cJSON *vers = cJSON_CreateString(version);
    cJSON_AddItemToObject(meta, "version", vers);

    // step 2: Configure
    cJSON_SetValuestring(item1, "alpha2");
    cJSON_bool item1_invalid = cJSON_IsInvalid(item1);
    cJSON_bool meta_is_obj = cJSON_IsObject(meta);

    // step 3: Operate and Validate
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 1);
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *got_version = cJSON_GetObjectItem(got_meta, "version");
    const char *ver_str = cJSON_GetStringValue(got_version);
    buffer[0] = ver_str ? ver_str[0] : 'X';
    buffer[1] = (char)('0' + ((item1_invalid + meta_is_obj) % 10));
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}