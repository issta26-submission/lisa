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
//<ID> 1462
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"name\":\"test\",\"values\":[1,2,3],\"nested\":{\"k\":\"v\"}}";
    cJSON *root = cJSON_Parse(json);

    // step 2: Configure
    cJSON *values = cJSON_GetObjectItem(root, "values");
    int count = cJSON_GetArraySize(values);
    cJSON *values_dup = cJSON_Duplicate(values, 1);
    cJSON *newroot = cJSON_CreateObject();
    cJSON_AddItemToObject(newroot, "copied_values", values_dup);
    cJSON *nested = cJSON_GetObjectItem(root, "nested");
    cJSON *kitem = cJSON_GetObjectItem(nested, "k");
    char *kstr = cJSON_GetStringValue(kitem);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(newroot);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = out_len ? out[0] : ' ';
    scratch[1] = (char)('0' + (count % 10));
    scratch[2] = kstr ? kstr[0] : 'n';
    scratch[3] = (char)('A' + (count % 26));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(newroot);
    // API sequence test completed successfully
    return 66;
}