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
//<ID> 487
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"alpha\",\"flag\":false,\"count\":3}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, sizeof(json) - 1, &parse_end, 1);
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON_bool name_is_string = cJSON_IsString(name_item);

    // step 2: Configure
    cJSON *new_flag = cJSON_CreateBool(name_is_string);
    cJSON_ReplaceItemInObject(root, "flag", new_flag);

    // step 3: Operate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t buf_size = 128;
    char *temp_buf = (char *)cJSON_malloc(buf_size);
    memset(temp_buf, 0, buf_size);
    temp_buf[0] = snapshot && snapshot[0] ? snapshot[0] : 0;
    temp_buf[1] = snapshot && snapshot[1] ? snapshot[1] : 0;
    temp_buf[2] = snapshot && snapshot[2] ? snapshot[2] : 0;
    cJSON *extra_flag = cJSON_CreateBool(temp_buf[0] == '{');
    cJSON_AddItemToObject(root, "extra", extra_flag);

    // step 4: Validate & Cleanup
    (void)cJSON_IsString(cJSON_GetObjectItem(root, "name"));
    cJSON_free(snapshot);
    cJSON_free(temp_buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}