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
//<ID> 291
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char initial_json[] = "{\"alpha\":3.5,\"beta\":[1,2],\"note\":\"orig\"}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end1 = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end1, 1);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON *raw_item = cJSON_CreateRaw("{\"rawValue\":123}");
    cJSON *num_item = cJSON_CreateNumber(0.0);
    double set_result = cJSON_SetNumberHelper(num_item, 99.5);

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "injected_raw", raw_item);
    cJSON_AddItemToObject(root, "set_number", num_item);
    cJSON_AddStringToObject(root, "status", "updated");

    // step 3: Operate / Validate
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, 256, 1);
    cJSON_Minify(buffer);
    const char *parse_end2 = NULL;
    cJSON *reparsed = cJSON_ParseWithLengthOpts(buffer, 256, &parse_end2, 1);
    cJSON *retrieved = cJSON_GetObjectItem(reparsed, "set_number");
    double retrieved_val = cJSON_GetNumberValue(retrieved);
    (void)set_result;
    (void)retrieved_val;
    (void)printed_ok;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(reparsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}