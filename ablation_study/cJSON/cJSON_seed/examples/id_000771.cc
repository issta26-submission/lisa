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
//<ID> 771
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(3.1415);

    // step 2: Configure
    cJSON_AddItemToObject(root, "value", num_item);
    cJSON_AddTrueToObject(root, "enabled");
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);

    // step 3: Operate & Validate
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_value_item = cJSON_GetObjectItem(parsed, "value");
    double extracted = cJSON_GetNumberValue(parsed_value_item);
    cJSON *roundtrip_num = cJSON_CreateNumber(extracted);
    cJSON_AddItemToObject(parsed, "roundtrip", roundtrip_num);
    double roundtrip_value = cJSON_GetNumberValue(cJSON_GetObjectItem(parsed, "roundtrip"));
    (void)roundtrip_value;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}