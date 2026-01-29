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
//<ID> 1362
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *str_item = NULL;
    cJSON *raw_item = NULL;
    cJSON *num_item = NULL;
    char *printed = NULL;
    void *buffer = NULL;
    size_t buffer_size = 64;
    int raw_flag = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString("replace_me");
    cJSON_AddItemToObject(root, "key", str_item);
    raw_item = cJSON_CreateRaw("{\"raw\":true}");
    cJSON_AddItemToObject(root, "rawdata", raw_item);

    // step 3: Operate / Validate
    num_item = cJSON_CreateNumber(3.14159);
    cJSON_ReplaceItemInObject(root, "key", num_item);
    raw_flag = (int)cJSON_IsRaw(raw_item);
    printed = cJSON_PrintUnformatted(root);
    buffer = cJSON_malloc(buffer_size);
    memset(buffer, 0, buffer_size);
    ((char *)buffer)[0] = printed[0];

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}