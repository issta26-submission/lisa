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
//<ID> 1675
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
    cJSON *note_str = cJSON_CreateString("example note");
    cJSON_AddItemToObject(root, "note", note_str);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", flag);

    // step 2: Configure
    cJSON *num_item = cJSON_AddNumberToObject(meta, "pi", 3.14159);

    // step 3: Operate and Validate
    cJSON_bool is_false = cJSON_IsFalse(flag);
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = (char)('0' + (int)is_false);
    buffer[1] = printed ? printed[0] : 'X';
    double extracted = cJSON_GetNumberValue(num_item);
    buffer[2] = (char)('0' + (int)extracted % 10);
    (void)buffer;
    (void)extracted;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}