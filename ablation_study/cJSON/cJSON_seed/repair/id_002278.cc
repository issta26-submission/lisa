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
//<ID> 2278
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *raw = cJSON_CreateRaw("unparsed:123");
    cJSON_AddItemToObject(root, "raw", raw);
    cJSON *value = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(root, "value", value);

    // step 2: Configure
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buflen, 1);

    // step 3: Operate and Validate
    cJSON *got_raw = cJSON_GetObjectItem(root, "raw");
    cJSON_bool is_raw = cJSON_IsRaw(got_raw);
    cJSON_AddItemToObject(meta, "is_raw", cJSON_CreateBool(is_raw));
    cJSON *got_value = cJSON_GetObjectItem(root, "value");
    double num = cJSON_GetNumberValue(got_value);
    cJSON_AddNumberToObject(meta, "num", num);
    cJSON_AddBoolToObject(meta, "printed", printed);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}