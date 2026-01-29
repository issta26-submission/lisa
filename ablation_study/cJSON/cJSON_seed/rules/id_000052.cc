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
//<ID> 52
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char json_literal[] = "{\"name\":\"example\",\"data\":123}";
    size_t len = sizeof(json_literal) - 1;
    const char *parse_end = NULL;
    cJSON *root = NULL;
    cJSON *data_item = NULL;
    cJSON *raw_replacement = NULL;
    cJSON *raw_extra = NULL;
    char *printed = NULL;

    // step 2: Setup (parse and prepare replacements)
    root = cJSON_ParseWithLengthOpts(json_literal, len, &parse_end, 1);
    data_item = cJSON_GetObjectItemCaseSensitive(root, "data");
    raw_replacement = cJSON_CreateRaw("999");
    raw_extra = cJSON_CreateRaw("\"extra-raw\"");

    // step 3: Operate / Validate (replace and add items, then print)
    cJSON_ReplaceItemViaPointer(root, data_item, raw_replacement);
    cJSON_AddItemToObject(root, "raw_entry", raw_extra);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}