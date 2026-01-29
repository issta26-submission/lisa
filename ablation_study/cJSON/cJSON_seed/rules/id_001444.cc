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
//<ID> 1444
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *num_item = NULL;
    cJSON *str_replacement = NULL;
    cJSON *null_item = NULL;
    cJSON *raw_item = NULL;
    char *printed = NULL;
    char prebuf[256];
    cJSON_bool replace_ok = 0;
    cJSON_bool is_null = 0;
    int final_check = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObjectCS(root, "answer", num_item);
    null_item = cJSON_CreateNull();
    cJSON_AddItemToObjectCS(root, "nothing", null_item);
    raw_item = cJSON_AddRawToObject(root, "embedded", "{\"inner\":true}");
    str_replacement = cJSON_CreateString("forty-two");

    // step 3: Operate / Validate
    replace_ok = cJSON_ReplaceItemViaPointer(root, num_item, str_replacement);
    is_null = cJSON_IsNull(null_item);
    printed = cJSON_Print(root);
    memset(prebuf, 0, sizeof(prebuf));
    final_check = (replace_ok != 0) + (is_null != 0);
    prebuf[0] = (char)('0' + (char)final_check);
    (void)raw_item;
    (void)final_check;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}