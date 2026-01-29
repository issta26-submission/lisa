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
//<ID> 329
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"config\":{\"threshold\":10}}";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, json_len);
    cJSON *created_num = cJSON_CreateNumber(2.718);
    cJSON *added_num_via_api = NULL;
    cJSON *retrieved_config = NULL;
    cJSON *retrieved_threshold = NULL;
    cJSON *retrieved_created = NULL;
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    double threshold = 0.0;
    double e_val = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "e", created_num);
    added_num_via_api = cJSON_AddNumberToObject(root, "pi", 3.14);

    // step 3: Operate / Validate
    retrieved_config = cJSON_GetObjectItem(root, "config");
    retrieved_threshold = cJSON_GetObjectItem(retrieved_config, "threshold");
    threshold = cJSON_GetNumberValue(retrieved_threshold);
    retrieved_created = cJSON_GetObjectItem(root, "e");
    e_val = cJSON_GetNumberValue(retrieved_created);
    printed = cJSON_PrintUnformatted(root);
    cJSON_PrintPreallocated(root, buffer, 256, 0);
    buffer[0] = (char)('0' + ((int)threshold % 10));
    buffer[1] = (char)('A' + ((int)e_val % 26));
    (void)added_num_via_api;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}