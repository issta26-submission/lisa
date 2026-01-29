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
//<ID> 1668
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"existing\": null}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_text, &parse_end, 1);

    // step 2: Configure
    cJSON *created_str = cJSON_CreateString("injected");
    cJSON_AddItemToObject(root, "newstr", created_str);
    cJSON *existing_item = cJSON_GetObjectItem(root, "existing");
    cJSON_bool added_ref = cJSON_AddItemReferenceToObject(root, "existing_alias", existing_item);

    // step 3: Operate and Validate
    cJSON_bool is_existing_null = cJSON_IsNull(existing_item);
    char *printed = cJSON_PrintUnformatted(root);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (int)is_existing_null);
    const char *newstr_val = cJSON_GetStringValue(cJSON_GetObjectItem(root, "newstr"));
    buffer[2] = newstr_val ? newstr_val[0] : 'Y';
    (void)parse_end;
    (void)added_ref;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}