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
//<ID> 57
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char json_text[] = "{\"alpha\":1,\"beta\":\"two\"}";
    const size_t json_len = sizeof(json_text) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_CreateObject();
    cJSON *parsed = cJSON_ParseWithLengthOpts(json_text, json_len, &parse_end, 1);
    cJSON *raw_item = cJSON_CreateRaw("{\"raw_key\":10}");
    cJSON *number_item = cJSON_CreateNumber(42.0);
    cJSON *replacement_raw = cJSON_CreateRaw("{\"raw_key\":20}");
    cJSON *fetched_raw = NULL;
    cJSON *fetched_parsed = NULL;
    char *printed = NULL;
    double answer_val = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON_AddItemToObject(root, "raw", raw_item);
    cJSON_AddItemToObject(root, "answer", number_item);

    // step 3: Operate / Validate
    fetched_raw = cJSON_GetObjectItemCaseSensitive(root, "raw");
    cJSON_ReplaceItemViaPointer(root, fetched_raw, replacement_raw);
    fetched_parsed = cJSON_GetObjectItemCaseSensitive(root, "parsed");
    answer_val = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(root, "answer"));
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}