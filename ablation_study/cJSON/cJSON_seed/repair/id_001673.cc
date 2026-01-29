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
//<ID> 1673
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
    cJSON *name_item = cJSON_CreateString("example");
    cJSON_AddItemToObject(meta, "name", name_item);
    cJSON_AddNumberToObject(root, "value", 3.14159);

    // step 2: Configure
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag_false", flag_false);
    cJSON_bool is_false = cJSON_IsFalse(flag_false);
    char *printed = cJSON_PrintBuffered(root, 256, 1);

    // step 3: Operate and Validate
    cJSON *meta_retrieved = cJSON_GetObjectItem(root, "meta");
    cJSON *name_retrieved = cJSON_GetObjectItem(meta_retrieved, "name");
    const char *name_str = cJSON_GetStringValue(name_retrieved);
    cJSON *value_retrieved = cJSON_GetObjectItem(root, "value");
    double value_num = cJSON_GetNumberValue(value_retrieved);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = name_str ? name_str[0] : 'X';
    buffer[1] = (char)('0' + ((int)value_num % 10));
    buffer[2] = (char)('0' + (int)is_false);
    (void)buffer;
    (void)meta_retrieved;
    (void)name_retrieved;
    (void)value_retrieved;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}