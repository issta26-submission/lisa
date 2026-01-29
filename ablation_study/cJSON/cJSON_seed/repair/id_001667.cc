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
//<ID> 1667
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"name\":\"initial\",\"meta\":null}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_text, &parse_end, 1);

    // step 2: Configure
    cJSON *new_alias = cJSON_CreateString("alias_value");
    cJSON_bool added_ref = cJSON_AddItemReferenceToObject(root, "alias", new_alias);

    // step 3: Operate and Validate
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    const char *name_str = cJSON_GetStringValue(name_item);
    cJSON *meta_item = cJSON_GetObjectItem(root, "meta");
    cJSON_bool meta_is_null = cJSON_IsNull(meta_item);
    char *printed = cJSON_PrintUnformatted(root);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = name_str ? name_str[0] : 'Y';
    buffer[2] = (char)('0' + (int)added_ref);
    buffer[3] = (char)('0' + (int)meta_is_null);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    (void)buffer;
    (void)parse_end;
    return 66; // API sequence test completed successfully
}