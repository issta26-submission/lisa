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
//<ID> 2320
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", data);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *sref = cJSON_CreateStringReference("ref-string");
    cJSON_AddItemToObject(meta, "ref", sref);
    cJSON_AddItemToArray(data, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(data, cJSON_CreateNumber(3.14159));
    cJSON_AddItemToArray(data, cJSON_CreateBool(1));
    cJSON *data_copy = cJSON_Duplicate(data, 1);
    cJSON_AddItemToObject(root, "data_copy", data_copy);

    // step 3: Operate
    cJSON *meta_ref = cJSON_GetObjectItem(meta, "ref");
    const char *ref_val = cJSON_GetStringValue(meta_ref);
    cJSON *first = cJSON_GetArrayItem(data, 0);
    const char *first_str = cJSON_GetStringValue(first);
    char *json = cJSON_PrintUnformatted(root);

    // step 4: Validate and Cleanup
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (ref_val && ref_val[0]) ? ref_val[0] : (first_str && first_str[0] ? first_str[0] : '\0');
    cJSON_free(json);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}