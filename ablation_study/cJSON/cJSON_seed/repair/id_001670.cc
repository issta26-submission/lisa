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
//<ID> 1670
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
    cJSON *label_str = cJSON_CreateString("example-label");
    cJSON_AddItemToObject(root, "label", label_str);

    // step 2: Configure
    cJSON *num_item = cJSON_AddNumberToObject(meta, "value", 314.15);
    cJSON *false_item = cJSON_AddFalseToObject(root, "ok");

    // step 3: Operate and Validate
    cJSON_bool is_false = cJSON_IsFalse(false_item);
    char *printed = cJSON_PrintBuffered(root, 256, (cJSON_bool)1);
    char buffer[32];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (int)is_false);
    buffer[2] = num_item ? (char)('0' + ((int)cJSON_GetNumberValue(num_item) % 10)) : 'N';
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}