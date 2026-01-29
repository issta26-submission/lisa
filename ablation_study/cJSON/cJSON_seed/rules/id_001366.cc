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
//<ID> 1366
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *name_item = NULL;
    cJSON *raw_item = NULL;
    cJSON *new_name = NULL;
    cJSON *number_item = NULL;
    char *printed = NULL;
    char *temp_buffer = NULL;
    cJSON_bool is_raw = 0;
    cJSON_bool replaced = 0;
    size_t copy_len = 0;

    temp_buffer = (char *)cJSON_malloc(256);
    memset(temp_buffer, 0, 256);
    root = cJSON_CreateObject();

    // step 2: Setup / Configure
    name_item = cJSON_CreateString("initial_name");
    cJSON_AddItemToObject(root, "name", name_item);
    raw_item = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON_AddItemToObject(root, "raw_field", raw_item);
    number_item = cJSON_CreateNumber(7.5);
    cJSON_AddItemToObject(root, "count", number_item);

    // step 3: Operate / Validate
    is_raw = cJSON_IsRaw(raw_item);
    new_name = cJSON_CreateString("final_name");
    replaced = cJSON_ReplaceItemInObject(root, "name", new_name);
    printed = cJSON_PrintUnformatted(root);
    copy_len = strlen(printed);
    memcpy(temp_buffer, printed, (copy_len < 255) ? copy_len : 255);
    temp_buffer[(copy_len < 255) ? copy_len : 255] = '\0';
    temp_buffer[0] = (char)('0' + (is_raw != 0));
    (void)replaced;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(temp_buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}