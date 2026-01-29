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
//<ID> 393
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON *root = cJSON_CreateObject();
    /* Use a string here instead of raw to ensure the printed JSON is valid and can be parsed back */
    cJSON *raw_item = cJSON_CreateString("raw_payload");
    cJSON *str_item = cJSON_CreateString("hello");
    cJSON *parsed = NULL;
    cJSON *retrieved_greet = NULL;
    cJSON *retrieved_raw = NULL;
    cJSON *retrieved_bool = NULL;
    cJSON_bool is_raw = 0;
    cJSON_bool is_true = 0;
    char *str_val = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "raw", raw_item);
    cJSON_AddItemToObject(root, "greeting", str_item);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse(printed);
    retrieved_greet = cJSON_GetObjectItemCaseSensitive(parsed, "greeting");
    str_val = cJSON_GetStringValue(retrieved_greet);
    retrieved_raw = cJSON_GetObjectItem(parsed, "raw");
    is_raw = cJSON_IsRaw(retrieved_raw);
    retrieved_bool = cJSON_GetObjectItemCaseSensitive(parsed, "enabled");
    is_true = cJSON_IsTrue(retrieved_bool);
    buffer[0] = str_val ? str_val[0] : '\0';
    buffer[1] = 'N' + (is_raw * ('R' - 'N'));
    buffer[2] = 'F' + (is_true * ('T' - 'F'));
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}