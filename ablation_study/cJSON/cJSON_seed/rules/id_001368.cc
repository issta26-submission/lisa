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
//<ID> 1368
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *raw_item = NULL;
    cJSON *name_item = NULL;
    cJSON *value_item = NULL;
    cJSON *replacement = NULL;
    cJSON_bool raw_flag = 0;
    cJSON_bool print_ok = 0;
    char *buffer = NULL;
    const int buf_len = 256;

    // step 2: Setup / Configure
    buffer = (char*)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    root = cJSON_CreateObject();
    raw_item = cJSON_CreateRaw("{\"inner\":true}");
    cJSON_AddItemToObject(root, "raw", raw_item);
    name_item = cJSON_CreateString("initial_name");
    cJSON_AddItemToObject(root, "name", name_item);
    value_item = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(root, "value", value_item);

    // step 3: Operate / Validate
    raw_flag = cJSON_IsRaw(raw_item);
    replacement = cJSON_CreateString("raw_replaced");
    cJSON_ReplaceItemInObject(root, "raw", replacement);
    print_ok = cJSON_PrintPreallocated(root, buffer, buf_len, 0);
    (void)raw_flag;
    (void)print_ok;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}