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
//<ID> 386
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON *root = cJSON_CreateObject();
    cJSON *parsed = NULL;
    cJSON *label_ref = NULL;
    cJSON *raw_item = NULL;
    cJSON *num_item = NULL;
    char *printed = NULL;
    char *reprinted = NULL;
    double count_val = 0.0;
    cJSON_bool raw_flag = 0;

    // step 2: Setup / Configure
    cJSON_AddNumberToObject(root, "count", 7.0);
    label_ref = cJSON_CreateStringReference("example_label");
    cJSON_AddItemToObject(root, "label", label_ref);
    raw_item = cJSON_CreateRaw("{\"nested\":true}");
    cJSON_AddItemToObject(root, "raw", raw_item);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    cJSON_Minify(printed);
    parsed = cJSON_Parse(printed);
    num_item = cJSON_GetObjectItem(parsed, "count");
    count_val = cJSON_GetNumberValue(num_item);
    cJSON_AddNumberToObject(parsed, "double_count", count_val * 2.0);
    raw_item = cJSON_GetObjectItem(parsed, "raw");
    raw_flag = cJSON_IsRaw(raw_item);
    cJSON_AddNumberToObject(parsed, "raw_flag", (double)raw_flag);
    cJSON_DeleteItemFromObjectCaseSensitive(parsed, "label");
    reprinted = cJSON_PrintUnformatted(parsed);
    buffer[0] = (char)('0' + (int)count_val);
    buffer[1] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(reprinted);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}