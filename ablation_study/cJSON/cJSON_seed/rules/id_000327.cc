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
//<ID> 327
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(42.0);
    const char *raw_json = "{\"parsed\":7.5}";
    size_t raw_len = (sizeof("{\"parsed\":7.5}") - 1);
    cJSON *parsed = NULL;
    cJSON *parsed_item = NULL;
    cJSON *retrieved_direct = NULL;
    double v_parsed = 0.0;
    double v_direct = 0.0;
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(child, "answer", num_item);
    cJSON_AddNumberToObject(root, "direct_num", 3.14);
    parsed = cJSON_ParseWithLength(raw_json, raw_len);

    // step 3: Operate / Validate
    parsed_item = cJSON_GetObjectItem(parsed, "parsed");
    retrieved_direct = cJSON_GetObjectItem(root, "direct_num");
    v_parsed = cJSON_GetNumberValue(parsed_item);
    v_direct = cJSON_GetNumberValue(retrieved_direct);
    int sum = (int)(v_parsed + v_direct);
    buffer[0] = (char)('A' + (sum % 26));
    buffer[1] = (char)('0' + (int)(v_direct));
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}