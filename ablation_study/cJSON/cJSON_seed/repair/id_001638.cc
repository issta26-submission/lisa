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
//<ID> 1638
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"config\":{\"flags\":[false,false],\"name\":\"sample\"}}";
    cJSON *root = cJSON_Parse(json);
    cJSON *config = cJSON_GetObjectItemCaseSensitive(root, "config");
    cJSON *flags = cJSON_GetObjectItemCaseSensitive(config, "flags");
    cJSON *name = cJSON_GetObjectItemCaseSensitive(config, "name");

    // step 2: Configure
    cJSON *new_true = cJSON_CreateBool(1);
    cJSON_AddItemToArray(flags, new_true);

    // step 3: Operate and Validate
    int count = cJSON_GetArraySize(flags);
    char *name_str = cJSON_GetStringValue(name);
    char buffer[32];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = name_str ? name_str[0] : 'X';
    buffer[1] = (char)('0' + (count % 10));

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}