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
//<ID> 1292
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"item\":{\"num\":42},\"other\":3}";
    cJSON *parsed = cJSON_Parse(json);

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "parsed", parsed);
    cJSON *replacement_num = cJSON_CreateNumber(99.5);

    // step 3: Operate
    cJSON *target_item = cJSON_GetObjectItem(parsed, "item");
    cJSON_ReplaceItemViaPointer(parsed, target_item, replacement_num);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}