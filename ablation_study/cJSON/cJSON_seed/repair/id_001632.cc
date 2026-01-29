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
//<ID> 1632
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"name\":\"example\",\"values\":[10,20,30],\"flag\":true}";
    cJSON *root = cJSON_Parse(json_text);
    cJSON *values = cJSON_GetObjectItemCaseSensitive(root, "values");
    cJSON *flag_item = cJSON_GetObjectItemCaseSensitive(root, "flag");

    // step 2: Configure
    cJSON_bool flag_val = cJSON_IsTrue(flag_item);
    cJSON *dup_flag = cJSON_CreateBool(flag_val);

    // step 3: Operate and Validate
    cJSON_AddItemToArray(values, dup_flag);
    char *out = cJSON_PrintUnformatted(root);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = out ? out[0] : 'E';
    buffer[1] = (char)('0' + (int)cJSON_GetArraySize(values));
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}