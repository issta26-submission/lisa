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
//<ID> 1660
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"alpha\": null, \"beta\": 123}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json, &parse_end, 1);
    cJSON *alpha_item = cJSON_GetObjectItem(root, "alpha");

    // step 2: Configure
    cJSON *added_str = cJSON_CreateString("added_value");
    cJSON_bool ref_added = cJSON_AddItemReferenceToObject(root, "added", added_str);

    // step 3: Operate and Validate
    cJSON_bool alpha_was_null = cJSON_IsNull(alpha_item);
    char *printed = cJSON_PrintUnformatted(root);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (int)alpha_was_null);
    buffer[2] = (char)('0' + (int)ref_added);
    (void)parse_end;
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}