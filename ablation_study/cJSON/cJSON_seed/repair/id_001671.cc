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
//<ID> 1671
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
    cJSON_AddNumberToObject(meta, "version", 3.14);
    cJSON *msg = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "message", msg);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "ok", flag_false);

    // step 2: Configure and Operate
    cJSON *sub = cJSON_AddObjectToObject(meta, "sub");
    cJSON_AddNumberToObject(sub, "count", 7.0);
    cJSON *version_item = cJSON_GetObjectItem(meta, "version");
    double version_val = cJSON_GetNumberValue(version_item);
    char *printed = cJSON_PrintBuffered(root, 128, 1);

    // step 3: Validate
    cJSON_bool is_false = cJSON_IsFalse(cJSON_GetObjectItem(root, "ok"));
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (int)version_val % 10);
    buffer[2] = (char)('0' + (int)is_false);
    (void)buffer;
    (void)version_item;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}