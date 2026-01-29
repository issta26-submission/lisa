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
//<ID> 775
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *count_num = cJSON_CreateNumber(42.5);
    cJSON_AddItemToObject(root, "count", count_num);
    cJSON *enabled_true = cJSON_AddTrueToObject(root, "enabled");

    // step 2: Configure (serialize and prepare buffer)
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);

    // step 3: Operate & Validate (parse and extract values)
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_count = cJSON_GetObjectItem(parsed, "count");
    double extracted_value = cJSON_GetNumberValue(parsed_count);
    cJSON *parsed_enabled = cJSON_GetObjectItem(parsed, "enabled");
    (void)extracted_value;
    (void)parsed_enabled;
    (void)enabled_true;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}