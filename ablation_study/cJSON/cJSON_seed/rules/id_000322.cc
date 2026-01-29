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
//<ID> 322
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json_literal[] = "{\"metrics\": {\"initial\": 1.5}}";
    size_t json_len = sizeof(json_literal) - 1;
    cJSON *root = cJSON_ParseWithLength(json_literal, json_len);
    cJSON *metrics = cJSON_GetObjectItem(root, "metrics");
    cJSON *initial_item = cJSON_GetObjectItem(metrics, "initial");
    double initial_value = cJSON_GetNumberValue(initial_item);
    cJSON *created_num = cJSON_CreateNumber(100.25);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);

    // step 2: Setup / Configure
    cJSON *added_direct = cJSON_AddNumberToObject(metrics, "added_direct", 7.25);
    cJSON_AddItemToObject(metrics, "added_created", created_num);

    // step 3: Operate / Validate
    cJSON *retrieved_direct = cJSON_GetObjectItem(metrics, "added_direct");
    cJSON *retrieved_created = cJSON_GetObjectItem(metrics, "added_created");
    double val_direct = cJSON_GetNumberValue(retrieved_direct);
    double val_created = cJSON_GetNumberValue(retrieved_created);
    int sum_int = (int)(initial_value + val_direct + val_created);
    buffer[0] = (char)('A' + (sum_int % 26));
    buffer[1] = (char)('0' + ((int)val_direct % 10));
    buffer[2] = (char)('0' + ((int)val_created % 10));

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}