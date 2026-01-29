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
//<ID> 1313
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json = "{\"alpha\": 1, \"beta\": {\"x\": 10}}";
    size_t json_len = sizeof("{\"alpha\": 1, \"beta\": {\"x\": 10}}") - 1;
    const char *parse_end = NULL;
    cJSON *parsed = NULL;
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *raw1 = NULL;
    cJSON *raw2 = NULL;
    cJSON *beta = NULL;
    cJSON *beta_copy = NULL;
    cJSON_bool raw1_is_raw = 0;
    cJSON_bool beta_equal = 0;

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);
    root = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(root, "collection");
    raw1 = cJSON_CreateRaw("RAW_PAYLOAD");
    raw2 = cJSON_CreateRaw("42");
    cJSON_AddItemToArray(arr, raw1);
    cJSON_AddItemToArray(arr, raw2);

    // step 3: Operate / Validate
    beta = cJSON_GetObjectItem(parsed, "beta");
    raw1_is_raw = cJSON_IsRaw(raw1);
    beta_copy = cJSON_Duplicate(beta, 1);
    beta_equal = cJSON_Compare(beta, beta_copy, 1);
    (void)parse_end;
    (void)raw1_is_raw;
    (void)beta_equal;

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    cJSON_Delete(beta_copy);
    // API sequence test completed successfully
    return 66;
}