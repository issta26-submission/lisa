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
//<ID> 391
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    cJSON *root = cJSON_CreateObject();
    cJSON *parsed = NULL;
    cJSON *raw_item = cJSON_CreateRaw("raw_payload");
    cJSON *str_item = cJSON_CreateString("hello");
    cJSON *retr_bool = NULL;
    cJSON *retr_str = NULL;
    cJSON *retr_raw = NULL;
    char *name_val = NULL;
    cJSON_bool raw_flag = 0;
    cJSON_bool true_flag = 0;

    // step 2: Setup / Configure
    cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON_AddItemToObject(root, "raw", raw_item);
    cJSON_AddItemToObject(root, "name", str_item);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse(printed);
    retr_bool = cJSON_GetObjectItemCaseSensitive(parsed, "enabled");
    retr_str = cJSON_GetObjectItem(parsed, "name");
    retr_raw = cJSON_GetObjectItemCaseSensitive(parsed, "raw");
    name_val = cJSON_GetStringValue(retr_str);
    raw_flag = cJSON_IsRaw(retr_raw);
    true_flag = cJSON_IsTrue(retr_bool);
    buffer[0] = name_val ? name_val[0] : '\0';
    buffer[1] = (char)('0' + (int)raw_flag);
    buffer[2] = (char)('0' + (int)true_flag);
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}