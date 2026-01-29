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
//<ID> 1636
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"flags\":[false]}";
    cJSON *root = cJSON_Parse(json);

    // step 2: Configure
    cJSON *flags = cJSON_GetObjectItemCaseSensitive(root, "flags");
    cJSON *b_true = cJSON_CreateBool(1);
    cJSON_AddItemToArray(flags, b_true);
    cJSON *b_false = cJSON_CreateBool(0);
    cJSON_AddItemToArray(flags, b_false);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    cJSON *flags_again = cJSON_GetObjectItemCaseSensitive(root, "flags");
    cJSON *first_item = cJSON_GetArrayItem(flags_again, 0);
    int is_first_true = (int)cJSON_IsTrue(first_item);
    char buffer[32];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = (char)('0' + is_first_true);
    buffer[1] = printed[0];

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}