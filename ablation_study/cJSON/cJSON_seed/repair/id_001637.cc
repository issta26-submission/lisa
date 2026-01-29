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
//<ID> 1637
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"flags\":[true,false],\"meta\":{\"active\":true}}";
    cJSON *root = cJSON_Parse(json);
    cJSON *flags = cJSON_GetObjectItemCaseSensitive(root, "flags");

    // step 2: Configure
    cJSON *newbool = cJSON_CreateBool(1);
    cJSON_AddItemToArray(flags, newbool);

    // step 3: Operate and Validate
    cJSON *meta = cJSON_GetObjectItemCaseSensitive(root, "meta");
    cJSON *active = cJSON_GetObjectItemCaseSensitive(meta, "active");
    cJSON_bool active_val = cJSON_IsTrue(active);
    char *printed = cJSON_PrintUnformatted(root);
    char buffer[32];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed[0];
    buffer[1] = (char)('0' + (int)active_val);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}