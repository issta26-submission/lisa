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
//<ID> 1639
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"settings\":{\"flags\":[false,false],\"meta\":{\"id\":42}},\"name\":\"example\"}";
    cJSON *root = cJSON_Parse(json);
    cJSON *settings = cJSON_GetObjectItemCaseSensitive(root, "settings");
    cJSON *flags = cJSON_GetObjectItemCaseSensitive(settings, "flags");

    // step 2: Configure
    cJSON *btrue = cJSON_CreateBool(1);
    cJSON *bfalse = cJSON_CreateBool(0);
    cJSON_AddItemToArray(flags, btrue);
    cJSON_AddItemToArray(flags, bfalse);

    // step 3: Operate and Validate
    int count = cJSON_GetArraySize(flags);
    cJSON *last = cJSON_GetArrayItem(flags, count - 1);
    cJSON *name = cJSON_GetObjectItemCaseSensitive(root, "name");
    const char *namestr = cJSON_GetStringValue(name);
    char buffer[32];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = (char)('0' + (count % 10));
    buffer[1] = (char)('0' + (int)cJSON_IsTrue(last));
    buffer[2] = namestr ? namestr[0] : 'X';
    char *printed = cJSON_PrintUnformatted(root);
    buffer[3] = printed ? printed[0] : 'Y';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}