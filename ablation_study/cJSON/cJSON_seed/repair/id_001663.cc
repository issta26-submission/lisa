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
//<ID> 1663
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"name\":\"original\",\"value\":123,\"nullable\":null}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_text, &parse_end, 1);

    // step 2: Configure
    cJSON *extra_str = cJSON_CreateString("extra");
    cJSON_AddItemToObject(root, "extra", extra_str);
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON_bool ref_added = cJSON_AddItemReferenceToObject(root, "alias", name_item);

    // step 3: Operate and Validate
    cJSON *null_item = cJSON_GetObjectItem(root, "nullable");
    cJSON_bool is_null = cJSON_IsNull(null_item);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = (char)('0' + (int)is_null);
    buffer[1] = parse_end ? parse_end[0] : 'X';
    buffer[2] = (char)('0' + (int)ref_added);
    (void)buffer;

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}