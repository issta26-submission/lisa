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
//<ID> 795
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *raw_item = NULL;
    cJSON *parsed_array = NULL;
    const char *version = NULL;
    int array_size = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    raw_item = cJSON_CreateRaw("[10,20,30]");
    cJSON_AddItemToObject(root, "raw_numbers", raw_item);
    version = cJSON_Version();
    parsed_array = cJSON_Parse(cJSON_GetStringValue(raw_item));

    // step 3: Operate / Validate
    array_size = cJSON_GetArraySize(parsed_array);
    cJSON_AddItemToObject(root, "numbers", parsed_array);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}