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
//<ID> 396
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
    cJSON *raw_item = cJSON_CreateRaw("{\"inner\":true}");
    cJSON *str_item = cJSON_CreateString("example_string");
    cJSON *retrieved_str = NULL;
    cJSON *retrieved_flag = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "raw", raw_item);
    cJSON_AddItemToObject(root, "name", str_item);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse(printed);
    retrieved_str = cJSON_GetObjectItem(parsed, "name");
    retrieved_flag = cJSON_GetObjectItem(parsed, "enabled");
    buffer[0] = cJSON_GetStringValue(retrieved_str)[0];
    buffer[1] = 'F' + cJSON_IsTrue(retrieved_flag) * ('T' - 'F');
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}